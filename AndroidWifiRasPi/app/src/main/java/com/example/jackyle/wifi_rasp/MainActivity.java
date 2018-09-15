package com.example.jackyle.wifi_rasp;

<<<<<<< HEAD
import android.os.AsyncTask;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.util.Log;

import java.io.DataOutputStream;
=======
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
>>>>>>> da830c54544b485418af5919556ffd1ace8040e3
import java.io.IOException;
import java.net.InetAddress;
import java.net.Socket;
import java.net.UnknownHostException;

public class MainActivity extends AppCompatActivity {
    //UI Element
<<<<<<< HEAD
    Button btnUp;
    Button btnDown;
=======
    VideoView video;
    Button btnUp;
    Button btnDown;
    Button btnLeft;
    Button btnRight;
>>>>>>> da830c54544b485418af5919556ffd1ace8040e3
    EditText txtAddress;
    Socket myAppSocket = null;
    public static String wifiModuleIp = "";
    public static int wifiModulePort = 0;
<<<<<<< HEAD
    public static String CMD = "0";
=======
    public static String CMD = "";
>>>>>>> da830c54544b485418af5919556ffd1ace8040e3
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        btnUp = (Button) findViewById(R.id.btnUp);
        btnDown = (Button) findViewById(R.id.btnDown);
<<<<<<< HEAD

        txtAddress = (EditText) findViewById(R.id.ipAddress);

=======
        video = (VideoView) findViewById(R.id.videoView3);
        txtAddress = (EditText) findViewById(R.id.ipAddress);


        PlayVideo();
>>>>>>> da830c54544b485418af5919556ffd1ace8040e3
        btnUp.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                getIPandPort();
                CMD = "S";
<<<<<<< HEAD
                Socket_AsyncTask cmd_increase_servo = new Socket_AsyncTask();
                cmd_increase_servo.execute();
=======
                Socket_AsyncTask EnviaCMD = new Socket_AsyncTask();
                EnviaCMD.execute();
>>>>>>> da830c54544b485418af5919556ffd1ace8040e3
            }
        });
        btnDown.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                getIPandPort();
                CMD = "N";
<<<<<<< HEAD
                Socket_AsyncTask cmd_increase_servo = new Socket_AsyncTask();
                cmd_increase_servo.execute();
=======
                Socket_AsyncTask EnviaCMD = new Socket_AsyncTask();
                try{
                    EnviaCMD.execute();
                }catch(Exception e){
                    System.out.println(e.toString());
                }

>>>>>>> da830c54544b485418af5919556ffd1ace8040e3
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
<<<<<<< HEAD
=======

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
>>>>>>> da830c54544b485418af5919556ffd1ace8040e3
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
