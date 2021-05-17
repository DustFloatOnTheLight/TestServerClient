#include "AccessService.h"

#include <QDateTime>
#include <QTextStream>
#include <QNetworkReply>
#include "GCoreHttpService.h"
#include "GLDFileUtils.h"

static AccessService *s_pAccessServiceInstance = nullptr;

const int c_sTimeOut = 5000;
const QString c_sLogFileName = "log.txt";

const QString c_sKeyIsAllowAccess = "IsAllowAccess";

AccessService::AccessService()
    : m_pLogFile(nullptr)
{
    QString sLogFilePath = exePath() + c_sLogFileName;
    m_pLogFile = new QFile(sLogFilePath);
    m_pLogFile->open(QFile::WriteOnly | QFile::Text | QFile::Append);
}

AccessService::~AccessService()
{
    m_pLogFile->close();
    freeAndNil(m_pLogFile);
}

AccessService* AccessService::instance()
{
    if (nullptr == s_pAccessServiceInstance)
    {
        s_pAccessServiceInstance = new AccessService;
    }
    return s_pAccessServiceInstance;
}

bool AccessService::accessServer(QString url, QString &data)
{
    try
    {
        GCoreHttpService oHttp;
        oHttp.setTimeOut(getTimeOut());

#define GET_REQUEST
#ifdef GET_REQUEST
        QString sRet = oHttp.httpGet(url);
#else
        // 给请求报文头添加数据
        QHash<QString, QString> oHeaderDataHash;
        oHeaderDataHash.insert(c_sKeyIsAllowAccess, "true");
        QString sRet = oHttp.httpPost(url, GStrings(), oHeaderDataHash);
#endif

        int nHttpErrCode = oHttp.getErrorCode();
        if (QNetworkReply::NoError == nHttpErrCode)
        {
            logInfo(sRet);
            data = sRet;
            return true;
        }
        else
        {
            return false;
        }
    }
    catch (...)
    {
        return false;
    }
}

int AccessService::getTimeOut()
{
    return c_sTimeOut;
}

void AccessService::logInfo(QString info)
{
    QTextStream oStream(m_pLogFile);
    oStream << QDate::currentDate().toString() << "\n";
    oStream << QTime::currentTime().toString() << "\n";
    oStream << info << "\n\n";
}
