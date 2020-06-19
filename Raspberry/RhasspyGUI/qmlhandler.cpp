#include "qmlhandler.h"

QMLHandler::QMLHandler(QObject *parent) : QObject(parent)
{
    //Set recording settings
    audioRecorder = new QAudioRecorder();
    audioSettings.setCodec("audio/pcm");
    audioSettings.setSampleRate(16000);
    audioSettings.setChannelCount(1);
    audioSettings.setQuality(QMultimedia::VeryHighQuality);
    audioSettings.setEncodingMode(QMultimedia::ConstantQualityEncoding);

    audioRecorder->setEncodingSettings(audioSettings, QVideoEncoderSettings(), "audio/x-wav");

    //Create the output file, if doesn't exist
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
    //When the qml is loaded, get link to the desired objects
    qmlObj = obj;
    btnRecord = obj->findChild<QQuickItem*>("btnRecord");
    labelDetected = obj->findChild<QQuickItem*>("labelDetected");

    QObject::connect(btnRecord, SIGNAL(clicked()),
                     this, SLOT(click()));
}

void QMLHandler::click()
{
    //Start recording
    if(recording == false)
    {
        qDebug() << "Start record";
        audioRecorder->record();
        recording = true;

        btnRecord->setProperty("text", "Stop record command");

    }else{//Stop recording

        qDebug() << "Stop record";
        btnRecord->setProperty("text", "Start record command");
        audioRecorder->stop();
        recording = false;

        //Send post request to Rhaspsy API
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
    //When the Rhasspy API answers

    qDebug() << "Request reply received";
    QByteArray answerData = reply->readAll();
    qDebug() << answerData;
    file->close();

    //Look at result

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

    //Create buffers to be filled
    unsigned char bufferModbus[12] = {00, 0x00, 00, 00, 00, 0x06, 0x01};
    QString bufferRealObject;

    bool modbus = false;
    bool realObject = false;

    if(result.contains("switch light"))
    {

        unsigned char tmp[] = {0x05, 00, 0x00, 0x00, 00};

        memcpy(bufferModbus+7, tmp, sizeof(tmp));

        qDebug() << "Switch light";

        if(result.contains("on"))
        {
            qDebug() << "ON";
            bufferModbus[10] = 0xFF;
        }else if(result.contains("off")){

            qDebug() << "OFF";
        }

        modbus = true;

    }else if(result.contains("switch radiator"))
    {
        qDebug() << "Switch radiator";

        unsigned char tmp[] = {0x05, 00, 0x03, 0x00, 00};

        memcpy(bufferModbus+7, tmp, sizeof(tmp));

        if(result.contains("on"))
        {
            qDebug() << "ON";
            bufferModbus[10] = 0xFF;
        }else if(result.contains("off")){

            qDebug() << "OFF";
        }

        modbus = true;

    }else if(result.contains("windows blinds"))
    {

        qDebug() << "Windows blinds";

        unsigned char tmp[] = {0x05, 00, 0x04, 0x00, 00};

        memcpy(bufferModbus+7, tmp, sizeof(tmp));

        if(result.contains("open"))
        {
            qDebug() << "Open";
            bufferModbus[10] = 0xFF;
            bufferRealObject = "relay on";
        }else if(result.contains("close")){

            qDebug() << "Close";
            bufferRealObject = "relay off";
        }

        realObject = true;
        modbus = true;

    }else if(result.contains("whats the temperature"))
    {
        qDebug() << "Temperature";

        //unsigned char tmp[] = {0x04, 00, 0x08, 0x00, 01};

        //memcpy(buffer+7, tmp, sizeof(tmp));

        bufferRealObject = "temp";
        realObject = true;
    }else if(result.contains("whats the humidity"))
    {
        qDebug() << "Humidité";

        //unsigned char tmp[] = {0x04, 00, 0x08, 0x00, 01};

        //memcpy(buffer+7, tmp, sizeof(tmp));

        bufferRealObject = "humi";
        realObject = true;
    }


    //Send the appropriate messages

    if(realObject == true)//Sensors and relay TCP connection
    {
        clientTCP = new Client("192.168.0.12", 1234);

        if(clientTCP->envoie(bufferRealObject.toStdString().c_str(), bufferRealObject.size()))
        {
            char answer[20];

            clientTCP->reception(answer, 20);

            if(bufferRealObject == "temp")
            {
                float temp;
                memcpy(&temp, answer, sizeof(float));
                qDebug() << temp;
                labelDetected->setProperty("text", "Speech detected: " + result +"\n Température: "+QString::number(temp)+"°C");

            }else if(bufferRealObject == "humi")
            {
                float humi;
                memcpy(&humi, answer, sizeof(float));
                qDebug() << humi;
                labelDetected->setProperty("text", "Speech detected: " + result +"\n Humidité: "+QString::number(humi)+"%");
            }

        }

        char bye[] = "bye";

        clientTCP->envoie(bye, sizeof(bye));

        delete clientTCP;
    }

    if(modbus == true)//Modbus connection
    {
        clientTCP = new Client("127.0.0.1", 502);

        modbusID++;
        memcpy(bufferModbus, &modbusID, 2);

        for(int i = 0; i < sizeof(bufferModbus); i++)
        {
            qDebug() << hex << bufferModbus[i];
        }

        if(clientTCP->envoie((char *)bufferModbus, sizeof(bufferModbus)))
        {
            unsigned char ack[sizeof(bufferModbus)];

            clientTCP->reception((char *)ack, sizeof(ack));

            for(int i = 0; i < sizeof(ack); i++)
            {
                qDebug() << hex << ack[i];
            }
        }

        delete clientTCP;

    }

}
