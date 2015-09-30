#ifndef TIMERFORFLAG_H
#define TIMERFORFLAG_H

#include <QObject>
#include<qtimer.h>
#include<QtSql>
#include<structs.h>

class Timerforflag : public QTimer
{
    Q_OBJECT
public:
    explicit Timerforflag(QSqlDatabase *db, QString deviceid, long label, labelsvector& in, ipvector &in2);
    ~Timerforflag();

signals:

public slots:
    void updateflag();
public:
    QString tdeviceid;
    long tlabel;
    QSqlDatabase *tdb;
    labelsvector& labels;
    ipvector &ips;
};

#endif // TIMERFORFLAG_H
