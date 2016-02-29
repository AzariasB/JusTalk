#include "actionlist.h"

ActionList::ActionList(QObject *parent) :
    QObject(parent)
{
}

void ActionList::addAction(const QString &regex, const char *member)
{
	//Add a pair to the list
    actions_.append(QPair<QRegExp,const char*>(QRegExp(regex),member));
}

bool ActionList::triggerActions(QString &ref,QObject *objBase)
{
	//For eahc registered actions
    foreach (auto p, actions_) {
        if(p.first.indexIn(ref) != -1){//See if the regex matches
             return QMetaObject::invokeMethod(objBase,p.second,Qt::DirectConnection,
                                       Q_ARG(QRegExp,p.first),
                                       Q_ARG(QString,ref));
             //If it does, call the function with the given context
        }
    }

    /*
     * No function were called
     * */
    return false;

}
