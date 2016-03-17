#include "infodialog.h"

InfoDialog::InfoDialog(QWidget *parent):
    QDialog(parent),
    infos_(),
    buttons_(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal)
{
    buttons_.setEnabled(false);
    QFormLayout form(this);

    form.addRow(new QLabel("Connection informations"));

    infos_["Pseudo"] = new QLineEdit("");
    infos_["Address"] = new QLineEdit("localhost");
    infos_["Port"] = new QLineEdit("4200");

    for(auto it = infos_.begin(); it != infos_.end(); ++it){
        QString label = it.key() + ":";
        connect(it.value(),SIGNAL(textEdited(QString)),this,SLOT(checkFields()));
        form.addRow(label,it.value());
    }

    form.addRow(&buttons_);

    connect(&buttons_,SIGNAL(accepted()),this,SLOT(accept()));
    connect(&buttons_,SIGNAL(rejected()),this,SLOT(reject()));

    window()->setFixedSize(window()->sizeHint());

    if(this->exec() != QDialog::Accepted){
        infos_.clear();
    }
}

void InfoDialog::checkFields()
{
    QString port = infos_["Port"]->text();
    QString adress = infos_["Address"]->text();
    QString pseudo = infos_["Pseudo"]->text();

    if(pseudo.isEmpty() ||
            port.toInt() == 0 ||
            adress!= "localhost" && ipValid.indexIn(adress) == -1){
        buttons_.setEnabled(false);
    }else{
        buttons_.setEnabled(true);
    }
}


QMap<QString,QString> InfoDialog::getInfos()
{
    QMap<QString,QString> infs;
    for(auto it = infos_.begin(); it != infos_.end(); ++it ){
        infs[it.key().toLower()] = it.value()->text();
    }
    return infs;
}
