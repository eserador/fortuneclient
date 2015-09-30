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

#ifndef CLIENT_H
#define CLIENT_H


#include <QTcpSocket>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include"structs.h"




QT_BEGIN_NAMESPACE
class QTcpSocket;
class QNetworkSession;
QT_END_NAMESPACE

class RfidDetecor:public QThread
{
    Q_OBJECT

public:
    RfidDetecor(QString hostname, quint16 portin,labelsvector &in, QObject *parent = 0);
    ~RfidDetecor(){/* emit  notconnected(hostName);qDebug()<<"destr";*/}
  long deviseid; //идентификациоонный номер рфид
  void HELOTAGP();

  QString hostName;
private slots:
  //  void HELOTAGP();
     void readFortune();
    void displayError(QAbstractSocket::SocketError socketError);
    void sessionOpened();
  //  void run();
  //  QString getmac();
signals:
    void datacame(rfidData &rfiddata);
    void notconnected(QString &ip);
    void connected(QString &ip);

private:

    quint16 port;
    QTcpSocket *tcpSocket;
    quint16 blockSize;
    QNetworkSession *networkSession;
    rfidData tmp;
    void datatohex(std::string&in, unsigned char* in2);
    unsigned char test[24];
    labelsvector &labelsv;



};
//! [0]

#endif
