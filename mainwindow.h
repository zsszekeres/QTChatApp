#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "communicationhandler.h"
#include "databasecontroller.h"
#include "myuser.h"
#include "newroomdialog.h"
#include "settings.h"
#include "treemodel.h"
#include "user.h"

#include <QDebug>
#include <QLabel>
#include <QMainWindow>
#include <QSettings>
#include <QStandardItem>
#include <QTextTableFormat>
#include <QScrollBar>
#include <QTextTable>
#include <QTranslator>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setTrTexts();
    void customizeUIElements();
    void connectSignalsToSlots();

    void openNewRoomDialog();
    void openSettingsDialog();
    void showContextMenu(QStandardItem* item, const QPoint &pos);
    void appendMessage(const QString &from, const QString &message);

    void connectToTheServer();
    void writeSettingsData();
    void readSettingsData();
    void loadLanguage(int langId);

private slots:
    void on_actionNew_room_triggered();
    void on_actionSettings_triggered();
    void on_actionExit_triggered();
    void on_modifiedSettings();
    void on_createNewRoom();
    void onReturnPressed();
    void onContextMenuRequested(const QPoint &pos);
    void onMyUserId();
    void onClientsOnline();
    void onRoomsOnline();
    void onMessageArrived();
    void onLeaveRoom();
    void onJoinRoom();
private:
    Ui::MainWindow *ui;
    DatabaseController *db_controller;
    CommunicationHandler *connection_handler;
    PacketController *packet_controller;
    NewRoomDialog *new_room_dialog = nullptr;
    Settings *settings_dialog = nullptr;

    QStandardItemModel *model = nullptr;
    QTextTableFormat table_format;

    QSettings *qSettings;

    bool isConnected = false;
    MyUser *myUser;
    int tmpRoomid;

    QString currentLanguage;
    QTranslator translator;
};
#endif // MAINWINDOW_H
