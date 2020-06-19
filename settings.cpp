#include "settings.h"
#include "ui_settings.h"

Settings::Settings(QWidget *parent, QString name, int lang) :
    QDialog(parent),
    ui(new Ui::Settings)
{
    ui->setupUi(this);
    setTrTexts();

    this->ui->lineEdit->setText(name);
    switch (lang) {
    case 0:
        this->ui->rBtn_EN->setChecked(true);
        this->lang = LANG_EN;
        break;
    case 1:
        this->ui->rBtn_HU->setChecked(true);
        this->lang = LANG_HU;
        break;
    default:
        break;
    }
}


Settings::~Settings()
{
    delete ui;
}

QString Settings::getUsername() const
{
    return username;
}

void Settings::setUsername(const QString value)
{
    username = value;
}

int Settings::getToggledLangId() const
{
    return this->lang;
}

void Settings::setTrTexts()
{
    this->setWindowTitle(tr("Settings"));
    this->ui->l_settings_title->setText(tr("Settings"));
    this->ui->l_settings_username->setText(tr("Username"));
    this->ui->groupBox_lang->setTitle(tr("Language"));
    this->ui->rBtn_EN->setText(tr("English"));
    this->ui->rBtn_HU->setText(tr("Hungarian"));
}

void Settings::on_lineEdit_textChanged(const QString &arg1)
{
    this->username = arg1;
}

void Settings::on_rBtn_EN_clicked()
{
    this->lang = LANG_EN;
}

void Settings::on_rBtn_HU_clicked()
{
    this->lang = LANG_HU;
}
