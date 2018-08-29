package com.example.jackyle.wifi_rasp;

import android.net.Uri;
import android.os.AsyncTask;
import android.provider.Settings;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.*;
import android.widget.Button;
import android.widget.EditText;
import android.util.Log;
import android.widget.MediaController;
import android.widget.VideoView;
import java.io.DataOutputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.net.InetAddress;
import java.net.Socket;
import java.net.UnknownHostException;

public class MainActivity extends AppCompatActivity {
    //UI Element
    VideoView video;
    Button btnUp;
    Button btnDown;
    Button btnLeft;
    Button btnRight;
    EditText txtAddress;
    Socket myAppSocket = null;
    public static String wifiModuleIp = "";
    public static int wifiModulePort = 0;
    public static String CMD = "";
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        btnUp = (Button) findViewById(R.id.btnUp);
        btnDown = (Button) findViewById(R.id.btnDown);
        video = (VideoView) findViewById(R.id.videoView3);
        txtAddress = (EditText) findViewById(R.id.ipAddress);


        PlayVideo();
        btnUp.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                getIPandPort();
                CMD = "S";
                Socket_AsyncTask EnviaCMD = new Socket_AsyncTask();
                EnviaCMD.execute();
            }
        });
        btnDown.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                getIPandPort();
                CMD = "N";
                Socket_AsyncTask EnviaCMD = new Socket_AsyncTask();
                try{
                    EnviaCMD.execute();
                }catch(Exception e){
                    System.out.println(e.toString());
                }

            }
        });

    }
    public void getIPandPort()
    {
        String iPandPort = txtAddress.getText().toString();
        Log.d("MYTEST","IP String: "+ iPandPort);
        String temp[]= iPandPort.split(":");
        wifiModuleIp = temp[0];
        wifiModulePort = Integer.valueOf(temp[1]);
        Log.d("MY TEST","IP:" +wifiModuleIp);
        Log.d("MY TEST","PORT:"+wifiModulePort);
    }

    public void PlayVideo(){
        try{
            video.setVideoPath("http://videocdn.bodybuilding.com/video/mp4/62000/62792m.mp4");
            //MediaController mediaController = new MediaController(this);
            //video.setMediaController(mediaController);
            video.requestFocus();
            video.start();
            video.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    if (video.isPlaying()){
                        video.resume();
                        }
                    }

                }
            );


        }catch (Exception e){
            System.out.println(e.toString());
        }
    }
    public class Socket_AsyncTask extends AsyncTask<Void,Void,Void>
    {
        Socket socket;

        @Override
        protected Void doInBackground(Void... params){
            try{
                InetAddress inetAddress = InetAddress.getByName(MainActivity.wifiModuleIp);
                socket = new java.net.Socket(inetAddress,MainActivity.wifiModulePort);
                DataOutputStream dataOutputStream = new DataOutputStream(socket.getOutputStream());
                dataOutputStream.writeBytes(CMD);
                dataOutputStream.close();
                socket.close();
            }catch (UnknownHostException e){e.printStackTrace();}catch (IOException e){e.printStackTrace();}
            return null;
        }
    }
}
