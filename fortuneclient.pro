QT += network sql

HEADERS       = \
    RfidDetecor.h \
    oraclebd.h \
    structs.h \
    timerforflag.h \
    composit.h

SOURCES       = \
                main.cpp \
    RfidDetecor.cpp \
    oraclebd.cpp \
    timerforflag.cpp \
    composit.cpp



#LIBS+=-L/home/oracle/oracle/instantclient_11_2 -lclntsh
#LIBS+=-L/home/oracle/oracle/instantclient_11_2 -lnnz11

# install
#
target.path = $$[QT_INSTALL_EXAMPLES]/network/fortuneclient
INSTALLS += target
