#include "timerforflag.h"

Timerforflag::Timerforflag(QSqlDatabase *db, QString deviceid, long label, labelsvector &in, ipvector &in2) : tdb(db),tdeviceid(deviceid),tlabel(label),labels(in),ips(in2)
{
connect(this,SIGNAL(timeout()),this,SLOT(updateflag()));
}

Timerforflag::~Timerforflag()
{

}
void Timerforflag::updateflag()
{
    if(labels.indexOf(tlabel)!=-1&&ips.indexOf(tdeviceid)!=-1)
    {
    QString sql;
    QSqlQuery *target;
    target=  new QSqlQuery("",*tdb);
    sql="UPDATE prior_inform SET FL_STATUS=1 WHERE IP_EQUIP='"+tdeviceid+"' AND NUM_METKA="+QString::number(tlabel);
    target->prepare ( sql );
    target->exec(sql);
    QString str1;
    str1=target->lastError().databaseText();
    qDebug()<<str1;
    delete target;
    }
    this->stop();
    qDebug()<<"timer!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!";

}
