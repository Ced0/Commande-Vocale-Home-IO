package com.example.commandevocal;

import androidx.appcompat.app.AppCompatActivity;

import android.os.AsyncTask;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import java.io.IOException;
import java.io.OutputStream;
import java.net.Socket;

public class MainActivity extends AppCompatActivity {

    Button btnLightOn;
    Button btnLightOff;
    Button btnRadiatorOn;
    Button btnRadiatorOff;
    Button btnOpenBlinds;
    Button btnCloseBlinds;

    TextView txt;
    Socket modbusSocket;
    OutputStream modbusSendStream;
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

            try {
                //Create a socket
                modbusSocket = new Socket("10.188.181.186", 502);
                modbusSendStream = modbusSocket.getOutputStream();

            } catch (IOException e) {
                e.printStackTrace();
                publishProgress("Error: " + e.getMessage());
                return "";
            }

            //Create and fill the buffer according to the command
            byte[] buffer = {00, 0x00, 00, 00, 00, 0x06, 0x01, 0x05, 00, 0x00, (byte)0x00, 00};

            Log.i("2-", command[0]);

            switch(command[0])
            {
                case "light on":
                    buffer[10] = (byte)0xFF;
                    break;
                case "close blinds":
                    buffer[9] = (byte)0x03;
                    buffer[10] = (byte)0x00;
                    break;
                case "open blinds":
                    buffer[9] = (byte)0x03;
                    buffer[10] = (byte)0xFF;
                    break;
                case "radiator on":
                    buffer[9] = (byte)0x02;
                    buffer[10] = (byte)0xFF;
                    break;
                case "radiator off":
                    buffer[9] = (byte)0x02;
                    buffer[10] = 0;
                    break;
                default:
                    return "";
                    //break;
            }

            //Send the message
            try {
                modbusSendStream.write(buffer);
            } catch (IOException e) {
                e.printStackTrace();
                publishProgress("Error: " + e.getMessage());
            }

            publishProgress("Everything alright !");

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
