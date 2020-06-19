package com.example.commandevocal;

import androidx.appcompat.app.AppCompatActivity;

import android.os.AsyncTask;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.Socket;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;

public class MainActivity extends AppCompatActivity {

    Button btnLightOn;
    Button btnLightOff;
    Button btnRadiatorOn;
    Button btnRadiatorOff;
    Button btnOpenBlinds;
    Button btnCloseBlinds;
    Button btnTemperature;
    Button btnHumidity;

    TextView txt;
    Socket modbusSocket;
    OutputStream modbusSendStream;
    Socket sensorSocket;
    OutputStream sensorSendStream;
    InputStream sensorReceiveStream;
    TacheAsynchrone asyncTask;

    // Debut de la classe TacheAsynchrone
    private class TacheAsynchrone extends AsyncTask<String, String, String> {

        // Méthode exécutée automatiquement avant l'execution de la tâche //asynchrone proprement dite
        // cette méthode est optionnelle, elle peut servir pour des initialisations
        @Override
        protected void onPreExecute() { }

        // Cette méthode permet d'afficher des résultats dans l'IU
        // à partir de données issues de la tâche en arrière plan
        // elle n'est pas obligatoire
        //@Override
        protected void onProgressUpdate(String[] result){
            txt.setText(result[0]);
        }


        @Override
        protected String doInBackground(String[] command) {

            //publishProgress();
            // Function to update progress to th UI

            boolean realObject = false;
            boolean modBus = false;

            //Create and fill the buffer according to the command
            byte[] modBusBuffer = {00, 0x00, 00, 00, 00, 0x06, 0x01, 0x05, 00, 0x00, (byte)0x00, 00};

            String sensorBuffer = "";

            Log.i("2-", command[0]);

            switch(command[0])
            {
                case "light on":
                    modBusBuffer[10] = (byte)0xFF;
                    modBus = true;
                    break;
                case "light off":
                    break;
                case "close blinds":
                    modBusBuffer[9] = (byte)0x04;
                    modBusBuffer[10] = (byte)0x00;
                    sensorBuffer = "relais off";
                    realObject = true;
                    modBus = true;
                    break;
                case "open blinds":
                    modBusBuffer[9] = (byte)0x04;
                    modBusBuffer[10] = (byte)0xFF;
                    sensorBuffer = "relais on";
                    realObject = true;
                    modBus = true;
                    break;
                case "radiator on":
                    modBusBuffer[9] = (byte)0x03;
                    modBusBuffer[10] = (byte)0xFF;
                    modBus = true;
                    break;
                case "radiator off":
                    modBusBuffer[9] = (byte)0x03;
                    modBusBuffer[10] = 0;
                    modBus = true;
                    break;
                case "temperature":
                    sensorBuffer = "temp";
                    realObject = true;
                    break;
                case "humidity":
                    sensorBuffer = "humi";
                    realObject = true;
                    break;
                default:
                    return "";
                    //break;
            }


            try {

                if(modBus == true)
                {
                    //Create a socket
                    modbusSocket = new Socket("192.168.0.48", 502);
                    modbusSendStream = modbusSocket.getOutputStream();

                    //Send the message
                    modbusSendStream.write(modBusBuffer);

                    publishProgress("Command send !");
                }

                if(realObject == true)
                {
                    //Create a socket
                    sensorSocket = new Socket("192.168.0.12", 1234);
                    sensorSendStream = sensorSocket.getOutputStream();

                    //Send the message
                    sensorSendStream.write(sensorBuffer.getBytes());

                    sensorReceiveStream = sensorSocket.getInputStream();
                    byte[] answerBuffer = new byte[20];
                    sensorReceiveStream.read(answerBuffer, 0, 20);
                    float result = ByteBuffer.wrap(answerBuffer).order(ByteOrder.LITTLE_ENDIAN).getFloat();

                    if(sensorBuffer == "temp")
                    {
                        publishProgress("Temperature: "+String.valueOf(result)+"°C");
                    }else if(sensorBuffer == "humi")
                    {
                        publishProgress("Humidity: "+String.valueOf(result)+"%");
                    }

                    //Disconnection message
                    sensorBuffer = "bye";
                    sensorSendStream.write(sensorBuffer.getBytes());
                }

            } catch (IOException e) {
                e.printStackTrace();
                publishProgress("Error: " + e.getMessage());
                return "";
            }


            return "";
        }

        // Méthode exécutée à la fin de l'execution de la tâche asynchrone
        // lors de l'exécution, le parametre resultat prend la valeur retournée par //doInBackground
        @Override
        protected void onPostExecute(String unused) {


        }
    }


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        btnLightOn = findViewById(R.id.btnLightOn);
        btnLightOff = findViewById(R.id.btnLightOff);
        btnRadiatorOn = findViewById(R.id.btnRadiatorOn);
        btnRadiatorOff = findViewById(R.id.btnRadiatorOff);
        btnCloseBlinds = findViewById(R.id.btnCloseBlinds);
        btnOpenBlinds = findViewById(R.id.btnOpenBlinds);
        btnTemperature = findViewById(R.id.btnTemperature);
        btnHumidity = findViewById(R.id.btnHumidity);
        txt = findViewById(R.id.textView);

        asyncTask = new TacheAsynchrone();

        btnLightOn.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {

                launchModbusTask("light on");
            }
        });

        btnLightOff.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {

                launchModbusTask("light off");
            }
        });

        btnRadiatorOn.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {

                launchModbusTask("radiator on");
            }
        });


        btnRadiatorOff.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {

                launchModbusTask("radiator off");
            }
        });

        btnCloseBlinds.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {

                launchModbusTask("close blinds");
            }
        });

        btnOpenBlinds.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {

                launchModbusTask("open blinds");
            }
        });

        btnTemperature.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {

                launchModbusTask("temperature");
            }
        });

        btnHumidity.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {

                launchModbusTask("humidity");
            }
        });
    }

    public void launchModbusTask(String command)//Launch the modbus thread
    {
        if(asyncTask.getStatus() == AsyncTask.Status.RUNNING) return;

        // si la tâche asynchrone précédente est terminée on en crée
        // une nouvelle, car on ne peut pas démarrer plusieurs fois
        // la même tâche
        if(asyncTask.getStatus() == AsyncTask.Status.FINISHED) {
            asyncTask = new TacheAsynchrone();
        }

        // on lance la tâche asynchrone suite à l’appui sur le bouton
        asyncTask.execute(command);
    }


}
