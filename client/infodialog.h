#ifndef INFODIALOG_H
#define INFODIALOG_H

#include <QObject>
#include <QDialog>
#include <QFormLayout>
#include <QMap>
#include <QLabel>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QCloseEvent>
#include <QMessageBox>
#include <QDebug>
#include <QRegExp>

class InfoDialog : public QDialog
{
    Q_OBJECT
public:
    InfoDialog(QWidget *parent = 0);

    QMap<QString,QString> getInfos();



private slots:

    void checkFields();

private:
    QMap<QString,QLineEdit*> infos_;

    QDialogButtonBox buttons_;

    const QRegExp ipValid = QRegExp("^((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$");
};

#endif // INFODIALOG_H
