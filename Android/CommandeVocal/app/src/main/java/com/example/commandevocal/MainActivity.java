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
import java.net.InetAddress;
import java.net.Socket;

public class MainActivity extends AppCompatActivity {

    Button justAButton;
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
        protected void onProgressUpdate(String result){
            txt.setText(result);
        }


        @Override
        protected String doInBackground(String[] notUsed) {

            //publishProgress(); //Function to update progress to th UI
            try {
                modbusSocket = new Socket("10.188.181.186", 502);
                modbusSendStream = modbusSocket.getOutputStream();

            } catch (IOException e) {
                e.printStackTrace();
                publishProgress("Error: " + e.getMessage());
                return "";
            }

            byte[] buffer = {00, 0x00, 00, 00, 00, 0x06, 0x01, 0x05, 00, 0x00, (byte)0xFF, 00};

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

        justAButton = findViewById(R.id.btnJustAButton);
        txt = findViewById(R.id.textView);

        asyncTask = new TacheAsynchrone();

        justAButton.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {

                if(asyncTask.getStatus() == AsyncTask.Status.RUNNING) return;

                // si la tâche asynchrone précédente est terminée on en crée
                // une nouvelle, car on ne peut pas démarrer plusieurs fois
                // la même tâche
                if(asyncTask.getStatus() == AsyncTask.Status.FINISHED) {
                    asyncTask = new TacheAsynchrone();
                }

                // on lance la tâche asynchrone suite à l’appui sur le bouton
                asyncTask.execute("Pass the command here !");

            }
        });
    }


}
