#include "db-settings-form.h"
#include "ui_dbsettingsform.h"

#include <QDir>
#include <QTextStream>
#include <QFileDialog>

#include "db.h"


DBSettingsForm::DBSettingsForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DBSettingsForm)
{
    ui->setupUi(this);

    QDir dir(QCoreApplication::applicationDirPath());
    dir.cd("db-scripts");

    QFileInfoList dirContent = dir.entryInfoList({"*.sql"});
    for(const auto &item : dirContent)
    {
        ui->convertScriptComboBox->addItem(item.fileName());
    }
}

DBSettingsForm::~DBSettingsForm()
{
    delete ui;
}

void DBSettingsForm::on_convertDBButton_clicked()
{
    QDir dir(QCoreApplication::applicationDirPath());
    dir.cd("db-scripts");

    QString fileName = dir.path() + "/" + ui->convertScriptComboBox->currentText();
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly))
        return;

    QTextStream in(&file);
    QStringList sqlScript = in.readAll().split(";");
    sqlScript.removeLast();

    file.close();
    DB::executeSqlQuery(sqlScript);
}

void DBSettingsForm::on_createDBButton_clicked()
{
    QString dbFileName = QFileDialog::getSaveFileName(
                this, tr("Choose DB"), "", tr("SQL Lite Files (*.sqldb *.db)"));

    if(dbFileName.isEmpty())
        return;

    DB::create(dbFileName);
}
