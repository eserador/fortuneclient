#ifndef ORACLEBD_H
#define ORACLEBD_H

#include <QObject>
#include <QtSql>
#include"structs.h"
//#include<qmessagebox.h>
#include<QMultiMap>
#include<qvector.h>
#include"timerforflag.h"


class Oraclebd: public QObject
{
     Q_OBJECT
public:
    Oraclebd();
    ~Oraclebd();
    bool createConnection();
public slots:
    bool insertrfiddata(rfidData &in);
    void selectrfidhistory();
    void selectipvector();
    void seleclabelsvector();
    void selectipvectort();
    void seleclabelsvectort();
    void update();
    void insertipnotconnected(QString &ip);
    void insertconnected(QString &ip);
    ipvector& getiPvector();
    labelsvector &getlabelsVector();
signals:
 void newip(QString &ip);
 void newlabel(long label);
 void ipdeleted(QString &ip);
 void labeldeleted(long label);
private:
    QSqlDatabase db;
    QMultiMap <QString,long> labelAndrfidId;
    QVector<Timerforflag*> timers;
    bool timerexist;
    ipvector iPvector;
    labelsvector labelsVector;
    QTimer *updatetimer;


};

#endif // ORACLEBD_H
