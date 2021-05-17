#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H

#include <QString>
#include <QFile>

class MainController
{
public:
    MainController();
    ~MainController();

    QString getServerData(QString url);

    QString loadLastAccessInfo();
    void saveAccessInfo(QString url);

private:
    QString m_sDataFromServer;
    QFile *m_pLastAccessFile;
};

#endif
