package com.example.jackyle.wifi_rasp;

import android.app.AlertDialog;
import android.content.Context;
import android.graphics.Color;
import android.media.MediaPlayer;
import android.net.Uri;
import android.os.AsyncTask;
import android.provider.Settings;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.*;
import android.webkit.WebSettings;
import android.webkit.WebView;
import android.webkit.WebViewClient;
import android.widget.Button;
import android.widget.EditText;
import android.util.Log;
import android.widget.MediaController;
import android.widget.Toast;
import android.widget.VideoView;
import java.io.DataOutputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.net.ConnectException;
import java.net.InetAddress;
import java.net.Socket;
import java.net.SocketException;
import java.net.UnknownHostException;
import java.util.ArrayList;
import java.util.Arrays;

public class MainActivity extends AppCompatActivity  {
    //UI Element
    WebView video;
    EditText txtAddress;
    Button btnUp, btnDown, btnLeft, btnRight;
    Socket myAppSocket = null;
    int RunUp = 0;
    int RunDw = 0;
    int RunLf = 0;
    int RunRg = 0;
    public static String wifiModuleIp = "";
    public static int wifiModulePort = 0;
    public static String CMD;
    Context context;
    int duration;
    public  Socket socket;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        wifiModuleIp = "192.168.0.100";
        wifiModulePort = 8000;

        btnUp = (Button) findViewById(R.id.btnUp);
        btnDown = (Button) findViewById(R.id.btnDown);
        btnLeft = (Button) findViewById(R.id.btnLeft);
        btnRight = (Button) findViewById(R.id.btnRight);
        video = (WebView) findViewById(R.id.webView);
        video.setVisibility(View.VISIBLE);
        PlayVideo();
        //Socket_Async socket =  new Socket_Async();
        //socket.execute();

       final AlertDialog.Builder builder = new AlertDialog.Builder(this);

        btnUp.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                switch (event.getAction()) {
                    case MotionEvent.ACTION_DOWN:
                        //ButtonAction("1UP");
                        builder.setTitle("1UP").show();

                        btnUp.setBackgroundColor(Color.MAGENTA);
                        break;
                    case MotionEvent.ACTION_UP:
                        //ButtonAction("0UP");
                        builder.setTitle("0UP").show();
                        btnUp.setBackgroundColor(Color.LTGRAY);
                        break;
                }
                return false;
            }
        });

        btnDown.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                switch (event.getAction()) {
                    case MotionEvent.ACTION_DOWN:
                        ButtonAction("1DW");
                        btnDown.setBackgroundColor(Color.MAGENTA);
                        break;
                    case MotionEvent.ACTION_UP:
                        ButtonAction("0DW");
                        btnDown.setBackgroundColor(Color.LTGRAY);
                        break;
                }
                return false;
            }
        });

        btnLeft.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                switch (event.getAction()) {
                    case MotionEvent.ACTION_DOWN:
                        ButtonAction("1LF");
                        btnLeft.setBackgroundColor(Color.MAGENTA);
                        break;
                    case MotionEvent.ACTION_UP:
                        ButtonAction("0LF");
                        btnLeft.setBackgroundColor(Color.LTGRAY);
                        break;
                }
                return false;
            }
        });


        btnRight.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                switch (event.getAction()) {
                    case MotionEvent.ACTION_DOWN:
                        ButtonAction("1RG");
                        btnRight.setBackgroundColor(Color.MAGENTA);
                        break;
                    case MotionEvent.ACTION_UP:
                        ButtonAction("0RG");
                        btnRight.setBackgroundColor(Color.LTGRAY);
                        break;
                }
                return false;
            }
        });
    }

    public void PlayVideo() {
        try {
            WebSettings webSettings = video.getSettings();
            webSettings.setJavaScriptEnabled(true);
            video.loadUrl("http://192.168.0.100:7000");
            video.setWebViewClient(new WebViewClient() {
                public void onReceivedError(WebView view, int errorCode, String description, String failingUrl) {
                    video.loadUrl("file:///android_asset/erro.html");
                }
            });
        } catch (Exception e) {
            System.out.println(e.toString());
        }
    }

    public boolean ButtonAction(String button) {
        CMD = button;
        Socket_AsyncTask EnviaCMD = new Socket_AsyncTask();
        try {
            EnviaCMD.execute();
        } catch (Exception e) {
            return false;
        }
        return true;
    }




    public class Socket_AsyncTask extends AsyncTask<Void,Void,Void> {
        @Override
        protected Void doInBackground(Void... params){
            try{
               // InetAddress inetAddress = InetAddress.getByName(MainActivity.wifiModuleIp);
               // socket = new java.net.Socket(inetAddress,MainActivity.wifiModulePort);
                DataOutputStream dataOutputStream = new DataOutputStream(socket.getOutputStream());
                dataOutputStream.writeBytes(CMD);
                dataOutputStream.close();
                socket.close();
            }catch (UnknownHostException e){e.printStackTrace();}catch (IOException e){e.printStackTrace();}
            return null;
        }
    }

    public class Socket_Async extends AsyncTask<Void,Void,Void> {
        @Override
        protected Void doInBackground(Void... params){
            try {

                InetAddress inetAddress = InetAddress.getByName(MainActivity.wifiModuleIp);
                socket = new java.net.Socket(inetAddress,MainActivity.wifiModulePort);

            } catch (SocketException e) {
                Toast(e,null,null);
            } catch (UnknownHostException e) {
                Toast(null,e,null);
            } catch (IOException e) {
                Toast(null,null,e);
            }
            return null;
        }

        public void Toast(SocketException e,UnknownHostException j,IOException f){
            context = getApplicationContext();
            duration = Toast.LENGTH_SHORT;
            Toast toast = null;
            if (e != null) {
                 toast = Toast.makeText(context, e.getMessage() , duration);
            }else if(j!= null){
                 toast = Toast.makeText(context, j.getMessage() , duration);
            }else if(f!= null){
                 toast = Toast.makeText(context, f.getMessage(), duration);
            }
            toast.show();
        }
    }
}


