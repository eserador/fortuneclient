#ifndef STRUCTS
#define STRUCTS
#include<qstring.h>
#include<qvector.h>
#include<QString>
struct rfidData
{
    QString time; // время срабатывания метки
    long labelid; //идентификационный номер метки
    QString deviceid; //идентификационный номер оборудования поймовщего метку
};


typedef QVector<long> labelsvector; // список меток находящихся под контролем системы
typedef QVector<QString> ipvector; // список ip находящихся под контролем системы

#endif // STRUCTS

