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
}


void QMLHandler::setQml(QObject *obj, const QUrl &objUrl)
{
    qmlObj = obj;
    btnRecord = obj->findChild<QQuickItem*>("btnRecord");
    labelDetected = obj->findChild<QQuickItem*>("labelDetected");

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

        btnRecord->setProperty("text", "Stop record command");

    }else{

        qDebug() << "Stop record";
        btnRecord->setProperty("text", "Start record command");
        audioRecorder->stop();
        recording = false;

        QNetworkAccessManager * mgr = new QNetworkAccessManager(this);
            connect(mgr,SIGNAL(finished(QNetworkReply*)),this,SLOT(onPostFinish(QNetworkReply*)));
            connect(mgr,SIGNAL(finished(QNetworkReply*)),mgr,SLOT(deleteLater()));

        if(file->open(QIODevice::ReadOnly) == true)
        {

            qDebug() << "Send post request";

            QNetworkReply* reply = mgr->post(QNetworkRequest(QUrl("http://169.254.36.97:12101/api/speech-to-intent?nohass=true")), file->readAll());

            qDebug() << "Wait for reply";
        }


    }
}

void QMLHandler::onPostFinish(QNetworkReply* reply)
{


    qDebug() << "Request reply received";
    QByteArray answerData = reply->readAll();
    qDebug() << answerData;
    file->close();



    QJsonDocument answerDoc = QJsonDocument::fromJson(answerData);

    QString result = "";

    if(answerDoc.isArray())
    {
        result = answerDoc.array().last()["raw_text"].toString();
    }else{
        result = answerDoc["raw_text"].toString();
    }

    qDebug() << result;

    labelDetected->setProperty("text", "Speech detected: " + result);

    unsigned char buffer[12] = {00, 0x00, 00, 00, 00, 0x06, 0x01};

    clientTCP = new Client("127.0.0.1", 502);

    modbusID++;
    memcpy(buffer, &modbusID, 2);

    if(result.contains("switch light"))
    {

        unsigned char tmp[] = {0x05, 00, 0x00, 0x00, 00};

        memcpy(buffer+7, tmp, sizeof(tmp));

        qDebug() << "Switch light";

        if(result.contains("on"))
        {
            qDebug() << "ON";
            buffer[10] = 0xFF;
        }else if(result.contains("off")){

            qDebug() << "OFF";
        }

    }else if(result.contains("switch radiator"))
    {
        qDebug() << "Switch radiator";

        unsigned char tmp[] = {0x05, 00, 0x03, 0x00, 00};

        memcpy(buffer+7, tmp, sizeof(tmp));

        if(result.contains("on"))
        {
            qDebug() << "ON";
            buffer[10] = 0xFF;
        }else if(result.contains("off")){

            qDebug() << "OFF";
        }

    }else if(result.contains("windows blinds"))
    {

        qDebug() << "Windows blinds";

        unsigned char tmp[] = {0x05, 00, 0x04, 0x00, 00};

        memcpy(buffer+7, tmp, sizeof(tmp));

        if(result.contains("open"))
        {
            qDebug() << "Open";
            buffer[10] = 0xFF;
        }else if(result.contains("close")){

            qDebug() << "Close";
        }

    }else if(result.contains("whats the temperature"))
    {
        qDebug() << "Temperature";

        unsigned char tmp[] = {0x04, 00, 0x08, 0x00, 01};

        memcpy(buffer+7, tmp, sizeof(tmp));
    }


    for(int i = 0; i < sizeof(buffer); i++)
    {
        qDebug() << hex << buffer[i];
    }

    if(clientTCP->envoie((char *)buffer, sizeof(buffer)))
    {
        unsigned char ack[sizeof(buffer)];

        clientTCP->reception((char *)ack, sizeof(ack));

        for(int i = 0; i < sizeof(ack); i++)
        {
            qDebug() << hex << ack[i];
        }
    }

    delete clientTCP;


}
