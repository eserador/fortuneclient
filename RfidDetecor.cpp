/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/


#include <QtNetwork>
#include "RfidDetecor.h"
//#include<string>
#include<string.h>


RfidDetecor::RfidDetecor(QString hostname,
quint16 portin, labelsvector &in, QObject *parent)
    :    networkSession(0),port(portin),hostName(hostname),deviseid(0),labelsv(in)
{

    tcpSocket = new QTcpSocket(this);
    tcpSocket->connectToHost(this->hostName,this->port);
    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(readFortune()));
    connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),this, SLOT(displayError(QAbstractSocket::SocketError)));
    QNetworkConfigurationManager manager;
    if (manager.capabilities() & QNetworkConfigurationManager::NetworkSessionRequired) {
        // Get saved network configuration
        QSettings settings(QSettings::UserScope, QLatin1String("QtProject"));
        settings.beginGroup(QLatin1String("QtNetwork"));
        const QString id = settings.value(QLatin1String("DefaultNetworkConfiguration")).toString();
        settings.endGroup();

        // If the saved network configuration is not currently discovered use the system default
        QNetworkConfiguration config = manager.configurationFromIdentifier(id);
        if ((config.state() & QNetworkConfiguration::Discovered) !=
            QNetworkConfiguration::Discovered) {
            config = manager.defaultConfiguration();
        }
        networkSession = new QNetworkSession(config, this);
        connect(networkSession, SIGNAL(opened()), this, SLOT(sessionOpened()));
        networkSession->open();
    }
   // if(tcpSocket->state()==QTcpSocket::ConnectingState)
   //     qDebug()<<"connected";
  //  else
   //     qDebug()<<"disconnected";




      deviseid=0;
      HELOTAGP();

}

void RfidDetecor::HELOTAGP()
{
    blockSize = 0;
    tcpSocket->write("HELOTAGP/1.1\n");
    tcpSocket->write("GET RF_SERNO\n");
    emit connected(hostName);

}



void RfidDetecor::readFortune()
{
    int id =0;
  static  char buff[1256];
     QString date;
    if (blockSize == 0) {
        if (tcpSocket->bytesAvailable() < (int)sizeof(quint16))
            return;
    }

    uint lench=tcpSocket->read(buff,sizeof(buff));
    std::string data=buff;

if(data.find("=")&&deviseid==0&&lench>25) // пакет с идетификациией оборудования
{
    size_t pos;
    pos = data.find("=") ;
    deviseid=QString::fromStdString(data.substr(pos+1,9)).toLong();


}
////   

    if (lench>=54)
    {
        size_t pos;
        pos = data.find("EVNTTAG");
        date=QString::fromStdString( data.substr(pos+8,17));
       std::string tmp2=data.substr(25,25);
       datatohex(tmp2,test);
       id = ((test[1]  & 0x3F) << 22) | (test[2] << 14) | (test[3] << 6) | ((test[4] & 0xFC) >> 2) ;
         tmp.deviceid=hostName;
         tmp.time=date;
         tmp.labelid=id;
         bool em=false;
         for(labelsvector::iterator i=labelsv.begin();i!=labelsv.end();i++)
         {
             if(tmp.labelid==*i)
                 em=true;
         }
        if(deviseid!=0&&em==true)
        {
         //qDebug()<<QString::number(tmp.labelid);
         emit datacame(tmp);
        }
        if(deviseid==0)
         tcpSocket->write("GET RF_SERNO\n");

    }
}

void RfidDetecor::displayError(QAbstractSocket::SocketError socketError)
{
    switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
        emit notconnected(hostName);
        break;
    case QAbstractSocket::HostNotFoundError:
        emit notconnected(hostName);
       qDebug()<< ("Rfid server "+this->hostName+ " The host was not found. Please check the  host name and port settings.");
        break;
    case QAbstractSocket::ConnectionRefusedError:
        emit notconnected(hostName);
         qDebug()<<"Rfid server "+this->hostName+ "The connection was refused by the peer. Make sure the fortune server is running,and check that the host name and port settings are correct.";
        break;
    default:
        emit notconnected(hostName);
         qDebug()<< "Rfid server "+this->hostName+ " The following error occurred: %1." +tcpSocket->errorString();
    }


}




void RfidDetecor::sessionOpened()
{
    // Save the used configuration
    QNetworkConfiguration config = networkSession->configuration();
    QString id;
    if (config.type() == QNetworkConfiguration::UserChoice)
        id = networkSession->sessionProperty(QLatin1String("UserChoiceConfiguration")).toString();
    else
        id = config.identifier();
    QSettings settings(QSettings::UserScope, QLatin1String("QtProject"));
    settings.beginGroup(QLatin1String("QtNetwork"));
    settings.setValue(QLatin1String("DefaultNetworkConfiguration"), id);
    settings.endGroup();

}
void RfidDetecor::datatohex(std::string&in,unsigned char* in2)
{
    QString test2=QString::fromStdString(in);

    int posfirst=0;
    int poslast=0;
    int in2pos=0;
while(in.find('%')<in.length())
{
    posfirst= in.find('%');
    in=in.replace(posfirst,1," ");
    poslast= in.find('%');

    if(in.substr(posfirst+1,poslast-posfirst-1).length()==2)
    {
        bool ok;
        in2[in2pos]=QString::fromStdString(in.substr(posfirst+1,poslast-posfirst-1)).toUInt(&ok,16);
        in2pos++;
    }
    if(in.substr(posfirst+1,poslast-posfirst-1).length()==3)
    {
        bool ok;
        in2[in2pos]=QString::fromStdString(in.substr(posfirst+1,2)).toUInt(&ok,16);
        in2pos++;
        in2[in2pos]=in.substr(posfirst+1+2,1).c_str()[0];
        in2pos++;

    }
    if(in.substr(posfirst+1,poslast-posfirst-1).length()==4)
    {
        bool ok;
        in2[in2pos]=QString::fromStdString(in.substr(posfirst+1,2)).toUInt(&ok,16);
        in2pos++;
        in2[in2pos]=in.substr(posfirst+1+2,1).c_str()[0];
        in2pos++;
        in2[in2pos]=in.substr(posfirst+1+2+1,1).c_str()[0];
        in2pos++;
    }
    if(in.substr(posfirst+1,poslast-posfirst-1).length()==5)
    {
        bool ok;
        in2[in2pos]=QString::fromStdString(in.substr(posfirst+1,2)).toUInt(&ok,16);
        in2pos++;
        in2[in2pos]=in.substr(posfirst+1+2,1).c_str()[0];
        in2pos++;
        in2[in2pos]=in.substr(posfirst+1+2+1,1).c_str()[0];
        in2pos++;
        in2[in2pos]=in.substr(posfirst+1+2+1,1).c_str()[0];
        in2pos++;
        in2[in2pos]=in.substr(posfirst+1+2+1+1,1).c_str()[0];
        in2pos++;
    }
}
}

