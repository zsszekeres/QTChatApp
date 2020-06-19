#include "newroomdialog.h"
#include "ui_newroomdialog.h"

#include <QDebug>

NewRoomDialog::NewRoomDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewRoomDialog)
{
    ui->setupUi(this);
    setTrTexts();
}

NewRoomDialog::~NewRoomDialog()
{
    delete ui;
}

QString NewRoomDialog::getNew_room() const
{
    return new_room;
}

void NewRoomDialog::setNew_room(const QString &value)
{
    qDebug() << "new Room name set to :" << value;
    new_room = value;
}

void NewRoomDialog::setTrTexts()
{
    this->setWindowTitle(tr("Add new room"));
    this->ui->l_new_room_dialog_text->setText(tr("Please give the name of the new room:"));
}

void NewRoomDialog::on_new_room_input_textChanged(const QString &arg1)
{
    setNew_room(arg1);
    qDebug() << "Roomname changed to: " << arg1;
}

void NewRoomDialog::on_buttonBox_accepted()
{
    this->ui->new_room_input->clear();
}
