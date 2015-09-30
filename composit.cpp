#include "composit.h"

composit::composit(QObject *parent) : QObject(parent)
{
    oraclebd=new Oraclebd;
    for(ipvector::iterator i=oraclebd->getiPvector().begin();i!=oraclebd->getiPvector().end();i++)
    {
            qDebug()<<*i;
            RfidDetecor *clientl=new RfidDetecor(*i,9999,oraclebd->getlabelsVector());
            QObject::connect(clientl,SIGNAL(datacame(rfidData&)),oraclebd,SLOT(insertrfiddata(rfidData&)));
            QObject::connect(clientl,SIGNAL(notconnected(QString&)),oraclebd,SLOT(insertipnotconnected(QString&)));
            QObject::connect(clientl,SIGNAL(connected(QString&)),oraclebd,SLOT(insertconnected(QString&)));
            clientl->HELOTAGP();
            rfiddetectors.push_back(clientl);
            clientl->hostName;
    }
    QObject::connect(oraclebd,SIGNAL(newip(QString&)),this,SLOT(createnewconnect(QString&)));
    QObject::connect(oraclebd,SIGNAL(ipdeleted(QString&)),this,SLOT(deleconnect(QString&)));
}

composit::~composit()
{

}
void composit::createnewconnect(QString &ip)
{
    qDebug()<<"+++++++++++++++++++++++++++++++++++++++++";
    qDebug()<<"connected"<<ip;
    RfidDetecor *clientl=new RfidDetecor(ip,9999,oraclebd->getlabelsVector());
    QObject::connect(clientl,SIGNAL(datacame(rfidData&)),oraclebd,SLOT(insertrfiddata(rfidData&)));
    QObject::connect(clientl,SIGNAL(notconnected(QString&)),oraclebd,SLOT(insertipnotconnected(QString&)));
    QObject::connect(clientl,SIGNAL(connected(QString&)),oraclebd,SLOT(insertconnected(QString&)));
    clientl->HELOTAGP();
    rfiddetectors.push_back(clientl);
}

void composit::deleconnect(QString &ip)
{
    qDebug()<<"-------------------------";
int id=0;
    for (QVector<RfidDetecor*>::iterator i=rfiddetectors.begin();i!=rfiddetectors.end();i++)
    {
        qDebug()<<QString::number(rfiddetectors.size());
        qDebug()<<(*i)->hostName<<"-----"<<ip;

         if((*i)->hostName==ip)
         {
            qDebug()<<"deleted:"<<ip;

             delete (*i);
             rfiddetectors.removeAt(id);
             return;
         }
         id++;
    }


}
