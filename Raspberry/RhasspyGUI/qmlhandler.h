#ifndef QMLHANDLER_H
#define QMLHANDLER_H

#include <QObject>
#include <QDebug>
#include <QQmlProperty>
#include <QAudioRecorder>
#include <QAudioEncoderSettings>
#include <QUrl>
#include <QFile>
#include <QCoreApplication>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QHttpMultiPart>
#include "client.h"
#include <QQuickItem>

class QMLHandler : public QObject
{
    Q_OBJECT
public:
    explicit QMLHandler(QObject *parent = nullptr);

public slots:
    void setQml(QObject *obj, const QUrl &objUrl);
    void click();
    void onPostFinish(QNetworkReply*);

signals:

private:
    QObject *qmlObj;
    QAudioRecorder* audioRecorder;
    QAudioEncoderSettings audioSettings;
    bool recording = false;
    QFile *file;
    Client* clientTCP;
    QQuickItem* btnRecord;
};

#endif // QMLHANDLER_H
