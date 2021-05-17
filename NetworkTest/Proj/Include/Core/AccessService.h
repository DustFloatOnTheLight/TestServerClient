#ifndef ACCESSSERVICE_H
#define ACCESSSERVICE_H

#include <QString>

class QFile;

class AccessService
{
public:
    AccessService();
    ~AccessService();

    static AccessService* instance();
    bool accessServer(QString url, QString& data);

private:
    int getTimeOut();
    void logInfo(QString info);

private:
    QFile *m_pLogFile;
};

#endif
