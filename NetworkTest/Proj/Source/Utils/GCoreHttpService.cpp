#include "GCoreHttpService.h"

#include <QEventLoop>
#include <QNetworkConfigurationManager>
#include <QNetworkReply>
#include <QPointer>
#include <QTextCodec>
#include "qcoreevent.h"            // for QEvent, QEvent::Type

#include "GBQServerConsts.h"        // for c_sCoreServiceVerNo, c_sCoreServi...
class QIODevice;

bool GWPTimerEventEater::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::Timer)
    {
        return true;
    }
    else
    {
        return QObject::eventFilter(obj, event);
    }
}

GCoreHttpService::GCoreHttpService()
{
    // m_header = "application/octet-stream";
    m_nTimeOut = MaxInt;
    m_nErrorCode = -1;
    m_pNetAccessMgr = new QNetworkAccessManager(this);
    connect(m_pNetAccessMgr, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(replyFinished(QNetworkReply*)));
}

GCoreHttpService::~GCoreHttpService()
{
    freeAndNil(m_pNetAccessMgr);
    QHash<QNetworkReply*, IHttpServiceController*>::iterator oIter = m_oCallBacks.begin();
    while (oIter != m_oCallBacks.end())
    {
        delete oIter.value();
        ++oIter;
    }
    m_oCallBacks.clear();
}

void GCoreHttpService::httpGet(const GString &url,
        QPointer<QNetworkReply> &networkReplyPointer,
        IHttpServiceController *callback)
{
    QNetworkRequest oRequest;
    oRequest.setRawHeader(c_sCoreServiceVersion.toUtf8(), c_sCoreServiceVerNo.toUtf8());
    oRequest.setUrl(QUrl(url));
    networkReplyPointer = m_pNetAccessMgr->get(oRequest);

    if (callback != NULL)
    {
        m_oCallBacks.insert(networkReplyPointer, callback);
    }
    else
    {
        waitForReplyFinished(networkReplyPointer);
    }
}

GString GCoreHttpService::httpGet(const GString &url)
{
    QNetworkRequest oRequest;
    oRequest.setRawHeader(c_sCoreServiceVersion.toUtf8(), c_sCoreServiceVerNo.toUtf8());
    oRequest.setUrl(QUrl(url));
    QNetworkReply *pReply = m_pNetAccessMgr->get(oRequest);
    waitForReplyFinished(pReply);

    QTextCodec *pCodec = QTextCodec::codecForName("utf8");
    GString sResult = pCodec->toUnicode(pReply->readAll());
    pReply->deleteLater();
    return sResult;
}


GString GCoreHttpService::httpPost(const GString &url, const GStrings &data, QHash<GString, GString> &headData)
{
    QNetworkRequest oRequest;
    oRequest.setRawHeader(c_sCoreServiceVersion.toUtf8(), c_sCoreServiceVerNo.toUtf8());
    foreach (GString sParamKey, headData.keys())
    {
        oRequest.setRawHeader(sParamKey.toUtf8(), headData.value(sParamKey).toUtf8());
    }

    oRequest.setUrl(QUrl(url));
    QString sData = "";
    for (int i = 0; i < data.length(); ++i)
    {
        if (sData == "")
        {
            sData = data.at(i);
        }
        else
        {
            sData = sData + "&" + data.at(i);
        }
    }

    QNetworkReply *pReply = m_pNetAccessMgr->post(oRequest, sData.toUtf8());
    waitForReplyFinished(pReply);

    QTextCodec *pCodec = QTextCodec::codecForName("utf8");
    GString sResult = pCodec->toUnicode(pReply->readAll());
    pReply->deleteLater();
    return sResult;
}

void GCoreHttpService::httpPost(const GString &url,
        const GStrings &source,
        QPointer<QNetworkReply> &networkReplyPointer,
        IHttpServiceController *callback)
{
    QNetworkRequest oRequest;
    oRequest.setRawHeader(c_sCoreServiceVersion.toUtf8(), c_sCoreServiceVerNo.toUtf8());
    oRequest.setUrl(QUrl(url.toUtf8()));

    QString sData = "";
    for (int i = 0; i < source.length(); ++i)
    {
        if (sData == "")
        {
            sData = source.at(i);
        }
        else
        {
            sData = sData + "&" + source.at(i);
        }
    }

    networkReplyPointer = m_pNetAccessMgr->post(oRequest, sData.toUtf8());

    if (callback != NULL)
    {
        m_oCallBacks.insert(networkReplyPointer, callback);
    }
    else
    {
        waitForReplyFinished(networkReplyPointer);
    }
}

void GCoreHttpService::httpPost(const GString &url,
        QIODevice *data,
        QPointer<QNetworkReply> &networkReplyPointer,
        IHttpServiceController *callback)
{
    QNetworkRequest oRequest;
    oRequest.setRawHeader(c_sCoreServiceVersion.toUtf8(), c_sCoreServiceVerNo.toUtf8());
    oRequest.setUrl(QUrl(url.toUtf8()));

    networkReplyPointer = m_pNetAccessMgr->post(oRequest, data);

    if (callback != NULL)
    {
        m_oCallBacks.insert(networkReplyPointer, callback);
    }
    else
    {
        waitForReplyFinished(networkReplyPointer);
    }
}

void GCoreHttpService::httpPost(const GString &url,
        const QByteArray &data,
        QPointer<QNetworkReply> &networkReplyPointer,
        IHttpServiceController *callback)
{
    QNetworkRequest oRequest;
    oRequest.setRawHeader(c_sCoreServiceVersion.toUtf8(), c_sCoreServiceVerNo.toUtf8());
    oRequest.setUrl(QUrl(url));
    oRequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    networkReplyPointer = m_pNetAccessMgr->post(oRequest, data);
    if (callback != NULL)
    {
        m_oCallBacks.insert(networkReplyPointer, callback);
    }
    else
    {
        waitForReplyFinished(networkReplyPointer);
    }
}

void GCoreHttpService::httpPost(const GString &url,
        QHttpMultiPart *multiPart,
        QPointer<QNetworkReply> &networkReplyPointer,
        IHttpServiceController *callback)
{
    QNetworkRequest oRequest;
    oRequest.setRawHeader(c_sCoreServiceVersion.toUtf8(), c_sCoreServiceVerNo.toUtf8());
    oRequest.setUrl(QUrl(url.toUtf8()));

    networkReplyPointer = m_pNetAccessMgr->post(oRequest, multiPart);

    if (callback != NULL)
    {
        m_oCallBacks.insert(networkReplyPointer, callback);
    }
    else
    {
        waitForReplyFinished(networkReplyPointer);
    }
}

void GCoreHttpService::httpPut(const GString &url,
        const GStrings &source,
        QPointer<QNetworkReply> &networkReplyPointer,
        IHttpServiceController *callback)
{
    QNetworkRequest oRequest;
    oRequest.setRawHeader(c_sCoreServiceVersion.toUtf8(), c_sCoreServiceVerNo.toUtf8());
    oRequest.setUrl(QUrl(url.toUtf8()));
    QString sData = "";
    for (int i = 0; i < source.length(); ++i)
    {
        if (sData == "")
        {
            sData = source.at(i);
        }
        else
        {
            sData = sData + "&" + source.at(i);
        }
    }

    networkReplyPointer = m_pNetAccessMgr->put(oRequest, sData.toUtf8());

    if (callback != NULL)
    {
        m_oCallBacks.insert(networkReplyPointer, callback);
    }
    else
    {
        waitForReplyFinished(networkReplyPointer);
    }
}

void GCoreHttpService::httpPut(const GString &url,
        QIODevice *data,
        QPointer<QNetworkReply> &networkReplyPointer,
        IHttpServiceController *callback)
{
    QNetworkRequest oRequest;
    oRequest.setRawHeader(c_sCoreServiceVersion.toUtf8(), c_sCoreServiceVerNo.toUtf8());
    oRequest.setUrl(QUrl(url));
    networkReplyPointer = m_pNetAccessMgr->put(oRequest, data);

    if (callback != NULL)
    {
        m_oCallBacks.insert(networkReplyPointer, callback);
    }
    else
    {
        waitForReplyFinished(networkReplyPointer);
    }
}

void GCoreHttpService::httpPut(const GString &url,
        const QByteArray &data,
        QPointer<QNetworkReply> &networkReplyPointer,
        IHttpServiceController *callback)
{
    QNetworkRequest oRequest;
    oRequest.setRawHeader(c_sCoreServiceVersion.toUtf8(), c_sCoreServiceVerNo.toUtf8());
    oRequest.setUrl(QUrl(url.toUtf8()));
    networkReplyPointer = m_pNetAccessMgr->put(oRequest, data);

    if (callback != NULL)
    {
        m_oCallBacks.insert(networkReplyPointer, callback);
    }
    else
    {
        waitForReplyFinished(networkReplyPointer);
    }
}

void GCoreHttpService::httpPut(const GString &url,
        QHttpMultiPart *multiPart,
        QPointer<QNetworkReply> &networkReplyPointer,
        IHttpServiceController *callback)
{
    QNetworkRequest oRequest;
    oRequest.setRawHeader(c_sCoreServiceVersion.toUtf8(), c_sCoreServiceVerNo.toUtf8());
    oRequest.setUrl(QUrl(url.toUtf8()));

    networkReplyPointer = m_pNetAccessMgr->put(oRequest, multiPart);

    if (callback != NULL)
    {
        m_oCallBacks.insert(networkReplyPointer, callback);
    }
    else
    {
        waitForReplyFinished(networkReplyPointer);
    }
}

void GCoreHttpService::httpDeleteResource(const GString &url,
        QPointer<QNetworkReply> &networkReplyPointer,
        IHttpServiceController *callback)
{
    QNetworkRequest oRequest;
    oRequest.setRawHeader(c_sCoreServiceVersion.toUtf8(), c_sCoreServiceVerNo.toUtf8());
    oRequest.setUrl(QUrl(url));

    networkReplyPointer = m_pNetAccessMgr->deleteResource(oRequest);

    if (callback != NULL)
    {
        m_oCallBacks.insert(networkReplyPointer, callback);
    }
    else
    {
        waitForReplyFinished(networkReplyPointer);
    }
}

void GCoreHttpService::setTimeOut(int msecs)
{
    m_nTimeOut = msecs;
}

int GCoreHttpService::getErrorCode()
{
    return m_nErrorCode;
}

void GCoreHttpService::replyFinished(QNetworkReply *reply)
{
    if (m_oCallBacks.value(reply) != NULL)
    {
        m_oCallBacks.value(reply)->replyFinished(reply);
    }

    m_oCallBacks.remove(reply);
}

void GCoreHttpService::replyDownloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    QNetworkReply *pReply = dynamic_cast<QNetworkReply *>(sender());
    QHash<QNetworkReply*, GReplyState*>::iterator oIter = m_oReplyStates.find(pReply);
    if (oIter != m_oReplyStates.end())
    {
        GReplyState *oState = oIter.value();
        oState->refreshDownloadState(bytesReceived, bytesTotal);
    }
}

void GCoreHttpService::replyUploadProgress(qint64 bytesSent, qint64 bytesTotal)
{
    QNetworkReply *pReply = dynamic_cast<QNetworkReply *>(sender());
    QHash<QNetworkReply*, GReplyState*>::iterator oIter = m_oReplyStates.find(pReply);
    if (oIter != m_oReplyStates.end())
    {
        GReplyState *oState = oIter.value();
        oState->refreshUploadState(bytesSent, bytesTotal);
    }
}

bool GCoreHttpService::waitForReplyFinished(QNetworkReply *reply)
{
    QEventLoop oEventLoop;
    GWPTimerEventEater oTimerEventEater;
    oEventLoop.installEventFilter(&oTimerEventEater);
    QTimer oTimer;
    oTimer.setSingleShot(true);
    GReplyState oReplyState(&oTimer, m_nTimeOut);
    m_oReplyStates.insert(reply, &oReplyState);

    connect(&oTimer, &QTimer::timeout, &oEventLoop, &QEventLoop::quit);
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), &oEventLoop, SLOT(quit()));
    connect(reply, SIGNAL(finished()), &oEventLoop, SLOT(quit()));
    connect(reply, SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(replyDownloadProgress(qint64, qint64)));
    connect(reply, SIGNAL(uploadProgress(qint64, qint64)), this, SLOT(replyUploadProgress(qint64, qint64)));

    oTimer.start(m_nTimeOut);
    oEventLoop.exec(QEventLoop::ExcludeUserInputEvents);

    if (!reply->isFinished())
    {
        m_nErrorCode = c_nTimeOutCode;
    }
    else
    {
        m_nErrorCode = reply->error();
    }

    m_oReplyStates.remove(reply);

    return reply->isFinished();
}

GCoreHttpService::GReplyState::GReplyState():
    timer(NULL)
{

}

GCoreHttpService::GReplyState::GReplyState(QTimer *timer, int timeOut):
    timer(timer), bytesReceived(0), bytesSent(0), noDataReceivedCount(0), noDataSentCount(0), timeOut(timeOut)
{
    time.start();
}

void GCoreHttpService::GReplyState::refreshDownloadState(qint64 bytesReceived, qint64 bytesTotal)
{
    if (bytesReceived == this->bytesReceived)
    {
        this->noDataReceivedCount += 1;
//        if (this->noDataReceivedCount == GReplyState::maxCount)
//        {
//            this->timer->setInterval(0);
//        }
    }
    else
    {
        this->bytesReceived = bytesReceived;
        int nTime = this->time.elapsed();
        if (nTime != 0)
        {
            int nSpeed = bytesReceived / nTime;
            qint64 nRemaining = bytesTotal - bytesReceived;
            if (nRemaining > 0 && nSpeed != 0)
            {
                int nRemainingTime = nRemaining / nSpeed * 2;
                if (nRemainingTime < 1000)
                {
                    this->timer->setInterval(nRemainingTime + 20000);
                }
                else
                {
                    this->timer->setInterval(nRemainingTime);
                }
            }
            else if (nRemaining == 0)
            {
                this->timer->setInterval(timeOut);
            }
        }
    }
}

void GCoreHttpService::GReplyState::refreshUploadState(qint64 bytesSent, qint64 bytesTotal)
{
    if (bytesSent == this->bytesSent)
    {
        this->noDataSentCount += 1;
//        if (this->noDataSentCount == GReplyState::maxCount)
//        {
//            this->timer->setInterval(0);
//        }
    }
    else
    {
        this->bytesSent = bytesSent;
        int nTime = this->time.elapsed();
        if (nTime != 0)
        {
            int nSpeed = bytesSent / nTime;
            qint64 nRemaining = bytesTotal - bytesSent;
            if (nRemaining > 0 && nSpeed != 0)
            {
                int nRemainingTime = nRemaining / nSpeed * 2;
                if (nRemainingTime < 1000)
                {
                    this->timer->setInterval(nRemainingTime + 20000);
                }
                else
                {
                    this->timer->setInterval(nRemainingTime);
                }
            }
            else if (nRemaining == 0)
            {
                this->timer->setInterval(timeOut);
            }
        }
    }
}
