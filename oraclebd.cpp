#include "oraclebd.h"

Oraclebd::Oraclebd()
{
createConnection();
}

Oraclebd::~Oraclebd()
{

}

bool Oraclebd::createConnection()
{
       db = QSqlDatabase::addDatabase("QOCI");
       db.setHostName("192.168.2.60");
       db.setDatabaseName("MINSK");
       db.setUserName("gai");
       db.setPassword("gai");
       if (!db.open()) {
                    qDebug()<< db.lastError().text();
           return false;
       }
       selectrfidhistory();
       selectipvector();
       seleclabelsvector();

       updatetimer=new QTimer;
       updatetimer->setInterval(10000);
       updatetimer->start();
       QObject::connect(updatetimer,SIGNAL(timeout()),this,SLOT(update()));

       return true;
}


 bool Oraclebd::insertrfiddata(rfidData &in)
{

   // qDebug()<<"timers size:"<<QString::number(timers.size());
    timerexist=false;
    QString sql;
    QString s_mac=in.deviceid;
    QString s_num_metka=QString::number(in.labelid);
    std::string s_date_tmp=in.time.toStdString();
    std::string s_date_tmp2;
    s_date_tmp2=s_date_tmp.substr(6,2)+"."+s_date_tmp.substr(4,2)+"."+s_date_tmp.substr(0,4)+"  "+
    s_date_tmp.substr(8,2)+":"+s_date_tmp.substr(10,2)+":"+s_date_tmp.substr(12,2);
    QString s_date=QString::fromStdString(s_date_tmp2);

    sql="insert into prior_inform (IP_EQUIP,NUM_METKA,DATA,DATA_EQUIP,FL_STATUS) values ('";
    sql+=s_mac + "',";
    sql+=s_num_metka + ",sysdate,"+"to_date('"+s_date+"','dd.MM.yyyy  hh24:mi:ss'),0)";
    QString str1;

     if(db.isOpen ()!=true)
     {
       return false;
     }
     else //База открыта
     {

       if(sql=="")
       {

         return false;
       }
       QSqlQuery *target;
       //bool b  =
       target=  new QSqlQuery("",db);
       target->prepare ( sql );
       //bool b1 =


       //

        for(QMultiMap<QString, long>::const_iterator i=labelAndrfidId.constBegin();i!=labelAndrfidId.constEnd();i++)
        {

              if(i.key()==in.deviceid&&i.value()==in.labelid)
              {
                   sql="UPDATE prior_inform SET FL_STATUS=0,DATA=sysdate,DATA_EQUIP=to_date('"+s_date+"','dd.MM.yyyy  hh24:mi:ss') WHERE IP_EQUIP='"+s_mac+"' "+"AND NUM_METKA="+s_num_metka;
                   target->exec(sql);
                   str1=target->lastError().databaseText();
                 //  qDebug()<<str1;
                   delete target;
                   for(QVector<Timerforflag*>::iterator i=timers.begin();i!=timers.end();i++)
                   {

                       if((*i)->tlabel==in.labelid&&(*i)->tdeviceid==in.deviceid)
                       {

                           (*i)->stop();
                           (*i)->setInterval(5000);
                           (*i)->start();
                           timerexist=true;
                       }
                   }
                                     if(!timerexist)
                                     {
                                         Timerforflag *tmp=new Timerforflag(&db,in.deviceid,in.labelid,labelsVector,iPvector);
                                         tmp->setInterval(5000);
                                         tmp->start();
                                         timers.push_back(tmp);
                                     }
                   //qDebug()<<sql;
                  return true;
              }

        }


       target->exec(sql);
       for(QVector<Timerforflag*>::iterator i=timers.begin();i!=timers.end();i++)
       {
           if((*i)->tlabel==in.labelid&&(*i)->tdeviceid==in.deviceid)
           {
               (*i)->stop();
               (*i)->setInterval(5000);
               (*i)->start();
               timerexist=true;
           }
       }
       if(!timerexist)
       {
           Timerforflag *tmp=new Timerforflag(&db,in.deviceid,in.labelid,labelsVector,iPvector);
           tmp->setInterval(5000);
           tmp->start();
           timers.push_back(tmp);
       }

       labelAndrfidId.insert(in.deviceid,in.labelid);

       str1=target->lastError().databaseText();
      // qDebug()<<str1;
       if(str1!="")
       {
      delete target;
      return false;
       }
    delete target;
     }//end else
     return true;


}

 void Oraclebd::selectrfidhistory()
 {
     QSqlQuery query;
         query.exec("SELECT IP_EQUIP,NUM_METKA FROM prior_inform");
         while (query.next()) {
                 QString mac = query.value(0).toString();
                 long metka = query.value(1).toLongLong();
                 labelAndrfidId.insert(mac,metka);
             }
 }
 void Oraclebd::selectipvector()
 {
     QSqlQuery query;
         query.exec("SELECT IP_EQUIP FROM PRIOR_EQUIP");
         while (query.next()) {
                  qDebug()<< query.value(0).toString();
               if(iPvector.indexOf(query.value(0).toString())==-1)
                   iPvector.push_back(query.value(0).toString());

               }
 }

 void Oraclebd::seleclabelsvector()
 {
     QSqlQuery query;
         query.exec("SELECT NUM_METKA FROM PRIOR_METKA");
         while (query.next()) {
                  qDebug()<< query.value(0).toString();
                  if(labelsVector.indexOf(query.value(0).toInt())==-1)
                      labelsVector.push_back(query.value(0).toInt());
             }

 }
 //
 void Oraclebd::selectipvectort()
 {
     ipvector newips;
     ipvector old=iPvector;
     QSqlQuery query;

     query.exec("SELECT IP_EQUIP FROM PRIOR_EQUIP");
         while (query.next()) {
                newips.push_back(query.value(0).toString());
             }
         int id=0;
         for(ipvector::iterator i=old.begin();i!=old.end();i++)
         {
            // qDebug()<<"*i"<<*i;
             if(newips.indexOf(*i)==-1)
             {
                 emit ipdeleted(*i);
                 iPvector.removeAt(id);

             }
             id++;

         }
        newips.clear();
     ///
     query.exec("SELECT IP_EQUIP FROM PRIOR_EQUIP");
         while (query.next()) {
                newips.push_back(query.value(0).toString());
               if(iPvector.indexOf(query.value(0).toString())==-1)
               {
                   iPvector.push_back(query.value(0).toString());
                  // old.push_back(query.value(0).toString());
                   QString ip=query.value(0).toString();
                   emit newip(ip);
               }
               }
        //
     //     iPvector=newips;

 }

 void Oraclebd::seleclabelsvectort()
 {
     labelsvector tmp;
     QSqlQuery query;
     query.exec("SELECT NUM_METKA FROM PRIOR_METKA");
         while (query.next()) {
             tmp.push_back(query.value(0).toInt());
                  if(labelsVector.indexOf(query.value(0).toInt())==-1)
                  {
                      labelsVector.push_back(query.value(0).toInt());
                      emit newlabel(query.value(0).toInt());
                  }
             }
         if(tmp!=labelsVector)
             labelsVector=tmp;

 }


 ipvector& Oraclebd::getiPvector()
 {
     return iPvector;

 }

 labelsvector &Oraclebd::getlabelsVector()
 {
     return labelsVector;
 }

 void Oraclebd::update()
 {
      selectipvectort();
      seleclabelsvectort();
 }
  void Oraclebd::insertipnotconnected(QString &ip)
  {

      //
      qDebug()<<ip;
      QSqlQuery query;
      query.exec("UPDATE PRIOR_DISREPAIR SET DATA=sysdate, STATUS=1 WHERE IP_EQUIP='"+ip+"'");
      qDebug()<<query.lastError().text();

  }
  void Oraclebd::insertconnected(QString &ip)
  {

      QSqlQuery query;
      query.exec("SELECT DATA PRIOR_DISREPAIR WHERE IP_EQUIP='"+ip+"'");
      if(query.next())
      {
         query.exec("UPDATE PRIOR_DISREPAIR SET DATA=sysdate, STATUS=0 WHERE IP_EQUIP='"+ip+"'");
      }
      else
      {
          QString que="insert into PRIOR_DISREPAIR (IP_EQUIP,DATA,STATUS) values ('"+ip+"',sysdate,0)";
          qDebug()<<que;
          query.exec(que);
      }
qDebug()<<query.lastError().text();
  }
