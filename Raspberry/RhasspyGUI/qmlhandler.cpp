#include "qmlhandler.h"

QMLHandler::QMLHandler(QObject *parent) : QObject(parent)
{
    audioRecorder = new QAudioRecorder();
    audioSettings.setCodec("audio/pcm");
    audioSettings.setSampleRate(16000);
    audioSettings.setChannelCount(1);
    audioSettings.setQuality(QMultimedia::VeryHighQuality);
    audioSettings.setEncodingMode(QMultimedia::ConstantQualityEncoding);

    audioRecorder->setEncodingSettings(audioSettings, QVideoEncoderSettings(), "audio/x-wav");

    file= new QFile(QCoreApplication::applicationDirPath() + "/record.wav");
    if(file->exists() == true)
    {
        file->open(QIODevice::NewOnly);
        file->close();
    }

    audioRecorder->setOutputLocation(QUrl::fromLocalFile(QCoreApplication::applicationDirPath() + "/record.wav"));

    //qDebug() << audioRecorder->outputLocation();
}


void QMLHandler::setQml(QObject *obj, const QUrl &objUrl)
{
    qmlObj = obj;
    QObject *bouton = obj->findChild<QObject*>("myItem");
    QObject::connect(bouton, SIGNAL(clicked()),
                     this, SLOT(click()));
}

void QMLHandler::click()
{
    if(recording == false)
    {
        qDebug() << "Start record";
        audioRecorder->record();
        recording = true;
    }else{
        qDebug() << "Stop record";
        audioRecorder->stop();
        recording = false;

        QNetworkAccessManager * mgr = new QNetworkAccessManager(this);
            connect(mgr,SIGNAL(finished(QNetworkReply*)),this,SLOT(onPostFinish(QNetworkReply*)));
            connect(mgr,SIGNAL(finished(QNetworkReply*)),mgr,SLOT(deleteLater()));

        if(file->open(QIODevice::ReadOnly) == true)
        {

            qDebug() << "Send post request";

            QNetworkReply* reply = mgr->post(QNetworkRequest(QUrl("http://192.168.0.12:12101/api/speech-to-intent?nohass=true")), file->readAll());

            qDebug() << "Wait for reply";
        }


    }
}

void QMLHandler::onPostFinish(QNetworkReply* reply)
{
    qDebug() << "Request reply received";
    qDebug() << reply->readAll();
    file->close();
}
