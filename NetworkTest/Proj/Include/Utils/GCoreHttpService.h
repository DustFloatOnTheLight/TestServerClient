/****************************************************************************************************

  作者: zhangl-u 2014.11.04
  备注: 通用网络访问类，支持同步、异步。
        shell代码中GHttpService的Core
  审核:

  Copyright (c) 2014-2020 Glodon Corporation

****************************************************************************************************/
#ifndef GCOREHTTPSERVICE_H
#define GCOREHTTPSERVICE_H
 
#pragma warning(disable: 4100)

#include <stddef.h>         // for NULL
#include <QTimer>

#include "GLDProject.h"     // for GLDPROJECTSHARED_EXPORT
#include "GLDString.h"      // for GString
#include "GLDStringList.h"  // for GStrings
#include "GLDTypes.h"
#include "qbytearray.h"     // for QByteArray
#include "qdatetime.h"      // for QTime
#include "qglobal.h"        // for qint64
#include "qhash.h"          // for QHash
#include "qobject.h"        // for QObject
#include "qobjectdefs.h"    // for Q_OBJECT, slots
#include "qstring.h"        // for QString

class QEvent;
class QIODevice;
class QTimer;
template <class T> class QPointer;

const int c_nTimeOutCode = 1000;
class QHttpMultiPart;
class QNetworkAccessManager;
class QNetworkReply;

class IHttpServiceController
{
public:
    virtual ~IHttpServiceController() {}

    virtual void replyFinished(QNetworkReply* reply) = 0;
};

class GWPTimerEventEater: public QObject
{
    Q_OBJECT
protected:
    bool eventFilter(QObject *obj, QEvent *event);
};


class GCoreHttpService : public QObject
{
    Q_OBJECT
public:
    GCoreHttpService();
    virtual ~GCoreHttpService();

    GString httpGet(const GString &url);
    GString httpPost(const GString &url, const GStrings &data, QHash<GString, GString> &headData);

    void httpGet(
            const GString &url,
            QPointer<QNetworkReply> &networkReplyPointer,
            IHttpServiceController *callback = NULL);
    void httpPost(
            const GString &url,
            const GStrings &source,
            QPointer<QNetworkReply> &networkReplyPointer,
            IHttpServiceController *callback = NULL);
    void httpPost(
            const GString &url,
            QIODevice *data,
            QPointer<QNetworkReply> &networkReplyPointer,
            IHttpServiceController *callback = NULL);
    void httpPost(
            const GString &url,
            const QByteArray &data,
            QPointer<QNetworkReply> &networkReplyPointer,
            IHttpServiceController *callback = NULL);
    void httpPost(
            const GString &url,
            QHttpMultiPart *multiPart,
            QPointer<QNetworkReply> &networkReplyPointer,
            IHttpServiceController *callback = NULL);

    void httpPut(
            const GString &url,
            const GStrings &source,
            QPointer<QNetworkReply> &networkReplyPointer,
            IHttpServiceController *callback = NULL);
    void httpPut(
            const GString &url,
            QIODevice *data,
            QPointer<QNetworkReply> &networkReplyPointer,
            IHttpServiceController *callback = NULL);
    void httpPut(
            const GString &url,
            const QByteArray &data,
            QPointer<QNetworkReply> &networkReplyPointer,
            IHttpServiceController *callback = NULL);
    void httpPut(
            const GString &url,
            QHttpMultiPart *multiPart,
            QPointer<QNetworkReply> &networkReplyPointer,
            IHttpServiceController *callback = NULL);
    void httpDeleteResource(
            const GString &url,
            QPointer<QNetworkReply> &networkReplyPointer,
            IHttpServiceController *callback = NULL);

    void setTimeOut(int msecs);
    int getErrorCode();

private slots:
    void replyFinished(QNetworkReply* reply);
    void replyDownloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void replyUploadProgress(qint64 bytesSent, qint64 bytesTotal);

private:

    struct GReplyState
    {
        GReplyState();
        GReplyState(QTimer *timer, int timeOut);

        void refreshDownloadState(qint64 bytesReceived, qint64 bytesTotal);
        void refreshUploadState(qint64 bytesSent, qint64 bytesTotal);

        QTimer *timer;
        QTime time;
        qint64 bytesSent;
        qint64 bytesReceived;
        int noDataReceivedCount;
        int noDataSentCount;
        int timeOut;
//        static const int maxCount = 5;
    };

    bool waitForReplyFinished(QNetworkReply *reply);

private:
    QNetworkAccessManager *m_pNetAccessMgr;
    QHash<QNetworkReply*, IHttpServiceController*> m_oCallBacks;
    QHash<QNetworkReply*, GReplyState*> m_oReplyStates;
    int m_nTimeOut;
    int m_nErrorCode;
};


#endif // GCOREHTTPSERVICE_H
