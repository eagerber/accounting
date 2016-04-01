#ifndef DBSETTINGSFORM_H
#define DBSETTINGSFORM_H

#include <QDialog>

namespace Ui {
class DBSettingsForm;
}

class DBSettingsForm : public QDialog
{
    Q_OBJECT

public:
    explicit DBSettingsForm(QWidget *parent = 0);
    ~DBSettingsForm();

private slots:
    void on_convertDBButton_clicked();

private:
    Ui::DBSettingsForm *ui;
    QString _dbFileName;
};

#endif // DBSETTINGSFORM_H
