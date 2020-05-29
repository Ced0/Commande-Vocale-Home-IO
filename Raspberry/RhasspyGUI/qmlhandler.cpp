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
    btnRecord = obj->findChild<QQuickItem*>("btnRecord");
    QObject::connect(btnRecord, SIGNAL(clicked()),
                     this, SLOT(click()));
}

void QMLHandler::click()
{
    if(recording == false)
    {
        qDebug() << "Start record";
        audioRecorder->record();
        recording = true;

        //((QPushButton *)btnRecord)->setText("Stop record command");
        btnRecord->setProperty("text", "Stop record command");
    }else{
        qDebug() << "Stop record";
        //((QPushButton *)btnRecord)->setText("Start record command");
        btnRecord->setProperty("text", "Start record command");
        audioRecorder->stop();
        recording = false;

        QNetworkAccessManager * mgr = new QNetworkAccessManager(this);
            connect(mgr,SIGNAL(finished(QNetworkReply*)),this,SLOT(onPostFinish(QNetworkReply*)));
            connect(mgr,SIGNAL(finished(QNetworkReply*)),mgr,SLOT(deleteLater()));

        if(file->open(QIODevice::ReadOnly) == true)
        {

            qDebug() << "Send post request";

            QNetworkReply* reply = mgr->post(QNetworkRequest(QUrl("http://169.254.140.106:12101/api/speech-to-intent?nohass=true")), file->readAll());

            qDebug() << "Wait for reply";
        }


    }
}

void QMLHandler::onPostFinish(QNetworkReply* reply)
{


    qDebug() << "Request reply received";
    qDebug() << reply->readAll();
    file->close();

    clientTCP = new Client("127.0.0.1", 502);

    //unsigned char buffer[] = {0x01, 0x01, 0xFF};
    unsigned char buffer[] = {00, 0x00, 00, 00, 00, 0x06, 0x01, 0x05, 00, 0x00, 0xFF, 00};

    char receive[20];

    clientTCP->envoie((char *)buffer, sizeof(buffer));
    /*clientTCP->reception(receive, 2);

    for(int i = 0; i < 20; i++)
    {
        qDebug() << receive[i];
    }*/


    delete clientTCP;
}
