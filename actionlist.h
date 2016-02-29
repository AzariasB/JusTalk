#ifndef ACTIONLIST_H
#define ACTIONLIST_H

#include <QObject>
#include <QList>
#include <QPair>
#include <QRegExp>
#include <QDebug>

class ActionList : public QObject
{
    Q_OBJECT
public:
    explicit ActionList(QObject *parent = 0);
    /**
     * @brief      add the action to the list of pair.
     * The string is used to build the regex.
     *
     * @param  regex   the regex base
     * @param  member  the name of the function to call if the regex matches
     */
    void addAction(const QString &regex,const char *member);

    /**
     * @brief      trigger the first action when the
     * string given in parameter matches
     *
     * @param      ref      the string to test
     * @param      objBase  the context object to test
     *
     * @return     the return value of the function, or false if no function was called
     */
    bool triggerActions(QString &ref, QObject *objBase);

signals:
public slots:

private:
	//All the action stored
    QList<QPair<QRegExp,const char*>> actions_;
};

#endif // ACTIONLIST_H
