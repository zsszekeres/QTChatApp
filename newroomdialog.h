#ifndef NEWROOMDIALOG_H
#define NEWROOMDIALOG_H

#include <QString>
#include <QDialog>

namespace Ui {
class NewRoomDialog;
}

class NewRoomDialog : public QDialog
{
    Q_OBJECT
public:
    explicit NewRoomDialog(QWidget *parent = nullptr);
    ~NewRoomDialog();

    QString getNew_room() const;
    void setNew_room(const QString &value);
    void setTrTexts();

private slots:
    void on_new_room_input_textChanged(const QString &arg1);

    void on_buttonBox_accepted();

private:
    Ui::NewRoomDialog *ui;
    QString new_room;
};

#endif // NEWROOMDIALOG_H
