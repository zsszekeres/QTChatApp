#include "mainwindow.h"
#include "ui_mainwindow.h"

// preferences
#define HOST_IP "193.6.19.52"
#define HOST_PORT 34152
#define COMPANY_NAME "ZsoltInc"
#define APP_NAME "KEK"
#define DB "db.sqlite"

// ui delay
#define STATUSBAR_TIMEOUT 8000

// language constants
#define EN "en"
#define HU "hu"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , model(new QStandardItemModel(this))

{
    this->myUser = new MyUser("");
    this->qSettings = new QSettings(COMPANY_NAME, APP_NAME);
    readSettingsData();

    ui->setupUi(this);

    this->packet_controller = new PacketController();
    this->connection_handler = new CommunicationHandler(HOST_IP, HOST_PORT, this->packet_controller);
    this->db_controller = new DatabaseController(DB);

    setTrTexts();
    customizeUIElements();
    loadLanguage(this->myUser->getLangId());
    connectSignalsToSlots();
}

MainWindow::~MainWindow()
{
    this->db_controller->~DatabaseController();
    delete ui;
}

void MainWindow::setTrTexts()
{
    this->setWindowTitle(tr("ChatApp"));
    this->ui->menuFile->setTitle(tr("File"));
    this->ui->actionNew_room->setText(tr("New room"));
    this->ui->actionSettings->setText(tr("Settings"));
    this->ui->actionExit->setText(tr("Exit"));
    this->ui->l_msg->setText(tr("Message:"));
    this->ui->l_room->setText(tr("Online rooms:"));
    this->ui->l_online_users->setText(tr("Online users:"));
}

void MainWindow::customizeUIElements()
{
    qDebug() << "UI options setting up..";

    this->ui->lineEdit->setFocusPolicy(Qt::StrongFocus);
    this->ui->textEdit->setFocusPolicy(Qt::NoFocus);
    this->ui->textEdit->setReadOnly(true);
    this->ui->onlineClients->setFocusPolicy(Qt::NoFocus);
    table_format.setBorder(0);

    this->ui->roomsTreeList->setHeaderHidden(true);
    this->ui->roomsTreeList->setContextMenuPolicy(Qt::CustomContextMenu);

    statusBar()->setFont(QFont ("Time", 8, QFont::SmallCaps));

    qDebug() << "UI options set up!";
}

void MainWindow::connectToTheServer()
{
    if (this->myUser->getName() != "") {
        this->connection_handler->sendMsg(this->packet_controller->buildSetUserData(this->myUser->getName()));
        statusBar()->showMessage(tr("Connecting to the server.."), STATUSBAR_TIMEOUT);
    } else {
        statusBar()->showMessage(tr("Please set username to connect to the server.."), STATUSBAR_TIMEOUT);
    }
}

void MainWindow::openNewRoomDialog()
{
    if(this->new_room_dialog == nullptr) {
        this->new_room_dialog = new NewRoomDialog(this);
        connect(new_room_dialog, SIGNAL(accepted()), this, SLOT(on_createNewRoom()));
    }
    this->new_room_dialog->show();
}

void MainWindow::openSettingsDialog()
{
    if (this->settings_dialog == nullptr) {
        this->settings_dialog = new Settings(this, this->myUser->getName(), this->myUser->getLangId());
        connect(settings_dialog, SIGNAL(accepted()), this, SLOT(on_modifiedSettings()));
    }
    this->settings_dialog->show();
}

void MainWindow::showContextMenu(QStandardItem *item, const QPoint &pos)
{
    QMenu *menu = new QMenu(this);
    Room r = this->packet_controller->getRoomById(item->data().toInt());
    //    qDebug() << "showContextMenu on room: " << r.getId();

    bool am_i_in = false;
    foreach (const User u, r.getUserList()) {
        if(u.getId() == this->myUser->getId()) {
            menu->addAction(tr("Leave"), this, SLOT(onLeaveRoom()));
            am_i_in = true;
        }
    }

    if(!am_i_in) {
        menu->addAction(tr("Join"), this, SLOT(onJoinRoom()));
    }

    menu->popup(this->ui->roomsTreeList->viewport()->mapToGlobal(pos));
}

void MainWindow::writeSettingsData()
{
    this->qSettings->beginGroup("MainWindow");

    this->qSettings->setValue("Name", this->myUser->getName());
    this->qSettings->setValue("Lang", this->myUser->getLangId());

    this->qSettings->endGroup();
}

void MainWindow::readSettingsData()
{
    this->qSettings->beginGroup("MainWindow");

    this->myUser->setName(this->qSettings->value("Name").toString());
    this->myUser->setLangId(this->qSettings->value("Lang").toInt());

    this->qSettings->endGroup();
}

void MainWindow::loadLanguage(int langId)
{
    QString lang;
    if (langId ==0) {
        lang = "en";
    } else if(langId == 1) {
        lang = "hu";
    }

    if (currentLanguage != lang) {
        if (!currentLanguage.isEmpty()){
            qApp->removeTranslator(&translator);
        }

        if (translator.load(lang+".qm")) {
            qApp->installTranslator(&translator);
            currentLanguage = lang;
            ui->retranslateUi(this);
            if (this->settings_dialog != nullptr)
                this->settings_dialog->setTrTexts();
            if (this->new_room_dialog != nullptr)
                this->new_room_dialog->setTrTexts();
        } else {
            // No such language to load
            qDebug() << "Language not found!";
            if (!currentLanguage.isEmpty()){
                translator.load(currentLanguage+".qm");
                qApp->installTranslator(&translator);
            }
        }
    }
}

void MainWindow::appendMessage(const QString &from, const QString &message)
{
    if(from.isEmpty() || message.isEmpty())
        return;

    QTextCursor cursor(this->ui->textEdit->textCursor());
    cursor.movePosition(QTextCursor::End);
    QTextTable* table = cursor.insertTable(1,2, table_format);
    table->cellAt(0,0).firstCursorPosition().insertText('<' + from + '>');
    table->cellAt(0,1).firstCursorPosition().insertText(message);
    QScrollBar *bar = this->ui->textEdit->verticalScrollBar();
    bar->setValue(bar->maximum());
}

// SLOTS

void MainWindow::on_actionNew_room_triggered()
{
    openNewRoomDialog();
}

void MainWindow::on_actionSettings_triggered()
{
    openSettingsDialog();
}

void MainWindow::on_actionExit_triggered()
{
    this->close();
}

void MainWindow::connectSignalsToSlots()
{
    connect(this->packet_controller, SIGNAL(myIdArrived()), this, SLOT(onMyUserId()));
    connect(this->packet_controller, SIGNAL(clientsOnline()), this, SLOT(onClientsOnline()));
    connect(this->packet_controller, SIGNAL(thereAreRooms()), this, SLOT(onRoomsOnline()));
    connect(this->packet_controller, SIGNAL(messageArrived()), this, SLOT(onMessageArrived()));
    connect(this->ui->lineEdit, SIGNAL(returnPressed()), this, SLOT(onReturnPressed()));
    connect(this->ui->roomsTreeList, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(onContextMenuRequested(QPoint)));
}

void MainWindow::on_modifiedSettings()
{
    if (this->myUser->getName() != this->settings_dialog->getUsername() && this->settings_dialog->getUsername() != "") {
        this->myUser->setName(this->settings_dialog->getUsername());
        if (!this->isConnected) {
            connectToTheServer();
        } else {
            this->connection_handler->sendMsg(this->packet_controller->buildSetUserData(this->myUser->getName()));
            statusBar()->showMessage(tr("Modify username.."), STATUSBAR_TIMEOUT);
        }
    }
    if(this->myUser->getLangId() != this->settings_dialog->getToggledLangId()) {
        this->myUser->setLangId(this->settings_dialog->getToggledLangId());
        qDebug() << "Changing language to: " << this->myUser->getLangId();
        loadLanguage(this->myUser->getLangId());
    }

    writeSettingsData();
}

void MainWindow::on_createNewRoom()
{
    onLeaveRoom();
    qDebug() << "New room name: " << this->new_room_dialog->getNew_room();
    this->connection_handler->sendMsg(this->packet_controller->buildNewRoomMsg(this->new_room_dialog->getNew_room()));
}

void MainWindow::onContextMenuRequested(const QPoint &pos)
{
    QStandardItem *asd;
    QModelIndex idx = this->ui->roomsTreeList->indexAt(pos);
    if (idx.isValid()) {
        asd = this->model->itemFromIndex(idx);
        showContextMenu(asd, pos);
        tmpRoomid = asd->data().toInt();
    }
}

void MainWindow::onMyUserId()
{
    this->myUser->setId(this->packet_controller->getMyid());
    statusBar()->showMessage(tr("Can communicate to the server!"), STATUSBAR_TIMEOUT);
    if (!this->isConnected) {
        connectToTheServer();
    }
}

void MainWindow::onClientsOnline()
{
    if (!this->isConnected) {
        this->isConnected = true;
        statusBar()->showMessage(tr("Connected!"), STATUSBAR_TIMEOUT);
    }
    while(this->ui->onlineClients->count()>0)
    {
        this->ui->onlineClients->takeItem(0);
    }
    foreach (const User u, this->packet_controller->getClients()) {
        this->ui->onlineClients->addItem(u.getName());
    }
}

void MainWindow::onRoomsOnline()
{
    QStandardItem *rootNode = this->model->invisibleRootItem();


    this->model->removeRows(0, this->model->rowCount());
    //foreach room
    foreach (const Room r, this->packet_controller->getRooms()) {

        QStandardItem *room_node = new QStandardItem(r.getName());
        QVariant var = QVariant(r.getId());
        room_node->setData(var);
        room_node->setFlags(room_node->flags() & ~Qt::ItemIsEditable);

        rootNode->appendRow(room_node);

        if (r.getUserList().size() > 0) {
            //foreach clients
            foreach (const User u, r.getUserList()) {
                // My user replace
                if (u.getId() == this->myUser->getId() && this->myUser->getRoomid() != r.getId()) {
                    this->myUser->setRoomid(r.getId());
                    if (!this->myUser->getInroom())
                        this->myUser->setInroom(true);
                }
                QStandardItem *user_node = new QStandardItem(u.getName());
                user_node->setFlags(user_node->flags() & ~Qt::ItemIsSelectable);
                room_node->appendRow(user_node);
            }
        }
    }

    this->ui->roomsTreeList->setModel(this->model);
    this->ui->roomsTreeList->expandAll();
}

void MainWindow::onReturnPressed()
{
    QString text = this->ui->lineEdit->text();
    if(text.isEmpty())
        return;

    if (text.startsWith(QChar('/'))) {
        QColor color = this->ui->textEdit->textColor();
        this->ui->textEdit->setTextColor(Qt::red);
        this->ui->textEdit->append(tr("! Unknown command: %1")
                                   .arg(text.left(text.indexOf(' '))));
        this->ui->textEdit->setTextColor(color);
    } else {
        //send msg
        if(this->myUser->getInroom())
            this->connection_handler->sendMsg(this->packet_controller->buildSendMessage(text, this->myUser->getRoomid()));
    }
    this->ui->lineEdit->clear();
}

void MainWindow::onMessageArrived()
{
    this->db_controller->insertMessage(this->packet_controller->getRoomid(), this->packet_controller->getArrivedMsg(), this->packet_controller->getUser()->getName());
    appendMessage(this->packet_controller->getUser()->getName(), this->packet_controller->getArrivedMsg());
}

void MainWindow::onLeaveRoom()
{
    if (this->myUser->getInroom()){
        this->connection_handler->sendMsg(this->packet_controller->buildEnterLeaveMsg(false, this->myUser->getRoomid()));
        this->myUser->setInroom(false);
        this->ui->textEdit->clear();
    }
}

void MainWindow::onJoinRoom()
{
    statusBar()->showMessage(tr("Joining to room (%1)...").arg(tmpRoomid),STATUSBAR_TIMEOUT);
//    if (this->myUser->getInroom()) {
//        this->connection_handler->sendMsg(this->packet_controller->buildEnterLeaveMsg(false, this->myUser->getRoomid()));
//        this->myUser->setInroom(false);
//        this->ui->textEdit->clear();
//    }
    onLeaveRoom();

    this->connection_handler->sendMsg(this->packet_controller->buildEnterLeaveMsg(true, this->tmpRoomid));
    this->myUser->setInroom(true);
    for(UserMessage usermsg: this->db_controller->listChatMessages(this->tmpRoomid)) {
        appendMessage(usermsg.getUsername(), usermsg.getContent());
    }
}

