#include "MainController.h"

#include <QTextStream>
#include "AccessService.h"
#include "GLDFileUtils.h"

const QString c_sErrorInfo = QStringLiteral("ÇëÇó·¢Éú´íÎó£¡");

const QString c_sLatAccessFileName = "lastAccess.txt";

MainController::MainController()
    : m_sDataFromServer(""),
      m_pLastAccessFile(nullptr)
{
    m_pLastAccessFile = new QFile(exePath() + c_sLatAccessFileName);
    m_pLastAccessFile->open(QIODevice::Text | QIODevice::ReadWrite);
}

MainController::~MainController()
{
    m_pLastAccessFile->close();
    freeAndNil(m_pLastAccessFile);
}

QString MainController::getServerData(QString url)
{
    saveAccessInfo(url);
    bool bStatus = AccessService::instance()->accessServer(url, m_sDataFromServer);
    if (!bStatus)
    {
        m_sDataFromServer = c_sErrorInfo;
    }
    return m_sDataFromServer;
}

QString MainController::loadLastAccessInfo()
{
    QTextStream oStream(m_pLastAccessFile);
    oStream.seek(0);

    return oStream.readLine();
}

void MainController::saveAccessInfo(QString url)
{
    m_pLastAccessFile->resize(0);
    QTextStream oStream(m_pLastAccessFile);
    oStream << url;
}
