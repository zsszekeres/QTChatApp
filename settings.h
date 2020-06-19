#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>
#include <QString>

// lang constants
#define LANG_EN 0
#define LANG_HU 1

namespace Ui {
class Settings;
}

class Settings : public QDialog
{
    Q_OBJECT
public:
    explicit Settings(QWidget *parent = nullptr, QString name = "", int lang = 0);
    ~Settings();

    void setUsername(const QString value);
    QString getUsername() const;
    int getToggledLangId() const;

    void setTrTexts();

private slots:
    void on_lineEdit_textChanged(const QString &arg1);
    void on_rBtn_EN_clicked();
    void on_rBtn_HU_clicked();

private:
    Ui::Settings *ui;

    QString username;
    int lang;
};

#endif // SETTINGS_H
