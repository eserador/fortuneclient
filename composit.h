#ifndef COMPOSIT_H
#define COMPOSIT_H

#include <QObject>
#include<oraclebd.h>
#include<RfidDetecor.h>

class composit : public QObject
{
    Q_OBJECT
public:
    explicit composit(QObject *parent = 0);
    ~composit();

signals:

public slots:
    void createnewconnect(QString &ip);
    void deleconnect(QString &ip);
private:
    Oraclebd *oraclebd;
    QVector<RfidDetecor*> rfiddetectors;
};

#endif // COMPOSIT_H
