#include "actionlist.h"

ActionList::ActionList(QObject *parent) :
    QObject(parent)
{
}


void ActionList::addAction(const QString &regex, const char *member)
{
    actions_.append(QPair<QRegExp,const char*>(QRegExp(regex),member));
}

bool ActionList::triggerActions(QString &ref,QObject *objBase)
{
    foreach (auto p, actions_) {
        if(p.first.indexIn(ref) != -1){
             return QMetaObject::invokeMethod(objBase,p.second,Qt::DirectConnection,
                                       Q_ARG(QRegExp,p.first),
                                       Q_ARG(QString,ref));
        }
    }

    /*
     * No function were called
     * */
    return false;

}
