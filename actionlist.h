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
    void addAction(const QString &regex,const char *member);
    bool triggerActions(QString &ref, QObject *objBase);

signals:
public slots:

private:
    QList<QPair<QRegExp,const char*>> actions_;
};

#endif // ACTIONLIST_H
