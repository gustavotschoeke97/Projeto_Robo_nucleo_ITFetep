package nucleo.itfetep.br.r2d2controller;

import android.graphics.Color;
import android.os.AsyncTask;
import android.os.Build;
import android.support.annotation.RequiresApi;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import java.io.DataOutputStream;
import android.view.*;
import android.webkit.WebSettings;
import android.webkit.WebView;
import android.webkit.WebViewClient;
import android.widget.ImageButton;
import android.widget.Toast;
import java.io.IOException;
import java.net.InetAddress;
import java.net.Socket;
import java.net.SocketException;
import java.net.UnknownHostException;

public class MainActivity extends AppCompatActivity  {
    //UI Element
    WebView video;
    ImageButton ImgBtnReturn;
    ImageButton ImgBtnturn;
    Button btnUp, btnDown, btnLeft, btnRight;
    ImageButton btnGole;
    public static String wifiIpRaspControl     = "";
    public static int wifiPortRaspControl      = 0  ;
    public static String wifiModuleIpRaspMedia = "";
    public static int wifiModulePortRasPMedia  = 0;
    public static String CMD;
    public int Return = 0;
    public int turn   = 0;
    public  Socket socket;
    boolean verifica;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        //UI componets and variables
        wifiIpRaspControl = "192.168.0.6";
        wifiPortRaspControl = 8000;
        wifiModuleIpRaspMedia = "192.168.0.100";
        wifiModulePortRasPMedia = 8000;

        btnUp        =   (Button) findViewById(R.id.btnUp);
        btnDown      =   (Button) findViewById(R.id.btnDown);
        btnLeft      =   (Button) findViewById(R.id.btnLeft);
        btnRight     =   (Button) findViewById(R.id.btnRight);
        ImgBtnReturn =   (ImageButton) findViewById(R.id.bt2);
        btnGole      =   (ImageButton) findViewById(R.id.imageButton);
        ImgBtnturn   =   (ImageButton) findViewById(R.id.imageButton2);
        video        =   (WebView) findViewById(R.id.webView);

        btnUp.setBackgroundColor(Color.rgb(54,73,158));
        btnUp.setTextColor(Color.WHITE);
        btnDown.setBackgroundColor(Color.rgb(54,73,158));
        btnDown.setTextColor(Color.WHITE);
        btnLeft.setBackgroundColor(Color.rgb(54,73, 158));
        btnLeft.setTextColor(Color.WHITE);
        btnRight.setBackgroundColor(Color.rgb(54,73,158));
        btnRight.setTextColor(Color.WHITE);


        PlayVideo();

        //Button actions
        ImgBtnReturn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if(Return == 0) {
                    Socket_AsyncTask EnviaRotacao = new Socket_AsyncTask("192.168.0.6", 8000, "90L");
                    EnviaRotacao.execute();
                    ImgBtnReturn.setBackgroundColor(Color.rgb(255, 165, 0));
                    Return = 1;
                }
                else if (Return == 1){

                    Socket_AsyncTask EnviaRotacao = new Socket_AsyncTask("192.168.0.6", 8000, "90C");
                    EnviaRotacao.execute();
                    ImgBtnReturn.setBackgroundColor(Color.rgb(54,73,158));
                    Return = 0;
                }
            }
        });

        ImgBtnturn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if(turn == 0) {
                    Socket_AsyncTask EnviaRotacao = new Socket_AsyncTask("192.168.0.6", 8000, "90R");
                    EnviaRotacao.execute();
                    ImgBtnturn.setBackgroundColor(Color.rgb(255, 165, 0));
                    turn = 1;
                }
                else if (turn == 1){
                    Socket_AsyncTask EnviaRotacao = new Socket_AsyncTask("192.168.0.6", 8000, "90C");
                    EnviaRotacao.execute();
                    ImgBtnturn.setBackgroundColor(Color.rgb(54,73,158));
                    turn = 0;
                }
            }
        });

        btnUp.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                switch (event.getAction()) {
                    case MotionEvent.ACTION_DOWN:
                        verifica = ButtonAction("1UP");
                        if(verifica == true) {
                            btnUp.setBackgroundColor(Color.rgb(255, 165, 0));
                            btnUp.setTextColor(Color.WHITE);
                        }
                        break;
                    case MotionEvent.ACTION_UP:
                        verifica = ButtonAction("0UP");
                        if(verifica == true) {
                            btnUp.setBackgroundColor(Color.rgb(54,73,158));
                            btnUp.setTextColor(Color.WHITE);
                        }
                        break;
                }
                return false;

            }
        });
        btnDown.setOnTouchListener(new View.OnTouchListener() {
            @RequiresApi(api = Build.VERSION_CODES.CUPCAKE)
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                switch (event.getAction()) {
                    case MotionEvent.ACTION_DOWN:
                        ButtonAction("1DW");
                        if(verifica == true) {
                            btnDown.setBackgroundColor(Color.rgb(255, 165, 0));
                            btnDown.setTextColor(Color.WHITE);
                        }
                        break;
                    case MotionEvent.ACTION_UP:
                        ButtonAction("0DW");
                        if(verifica == true) {
                            btnDown.setBackgroundColor(Color.rgb(54,73,158));
                            btnDown.setTextColor(Color.WHITE);
                        }
                        break;
                }
                return false;
            }
        });

        btnLeft.setOnTouchListener(new View.OnTouchListener() {
            @RequiresApi(api = Build.VERSION_CODES.CUPCAKE)
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                switch (event.getAction()) {
                    case MotionEvent.ACTION_DOWN:
                        ButtonAction("1LF");
                        if(verifica == true) {
                            btnLeft.setBackgroundColor(Color.rgb(255, 165, 0));
                            btnLeft.setTextColor(Color.WHITE);
                        }
                        break;
                    case MotionEvent.ACTION_UP:
                        ButtonAction("0LF");
                        if(verifica == true) {
                            btnLeft.setBackgroundColor(Color.rgb(54,73,158));
                            btnLeft.setTextColor(Color.WHITE);
                        }
                        break;
                }
                return false;
            }
        });


        btnRight.setOnTouchListener(new View.OnTouchListener() {
            @RequiresApi(api = Build.VERSION_CODES.CUPCAKE)
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                switch (event.getAction()) {
                    case MotionEvent.ACTION_DOWN:
                        ButtonAction("1RG");
                        if(verifica == true) {
                            btnRight.setBackgroundColor(Color.rgb(255, 165, 0));
                            btnRight.setTextColor(Color.WHITE);
                        }
                        break;
                    case MotionEvent.ACTION_UP:
                        ButtonAction("0RG");
                        if(verifica == true) {
                            btnRight.setBackgroundColor(Color.rgb(54,73,158));
                            btnRight.setTextColor(Color.WHITE);
                        }
                        break;
                }
                return false;
            }
        });

        btnGole.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
               final Socket_AsyncTask ChamaGole = new Socket_AsyncTask("192.168.0.100",8000,"1GO");
                new Thread() {

                    @Override
                    public void run() {
                        ChamaGole.execute();
                    }
                }.start();

            }
        });
    }

    //get video Stream from Mjpg local web page by Url
    public void PlayVideo() {
        try {
            video.setVisibility(View.VISIBLE);
            WebSettings webSettings = video.getSettings();
            webSettings.setJavaScriptEnabled(true);
            video.loadUrl("http://192.168.0.100:7000/stream_simple.html");
            video.setWebViewClient(new WebViewClient() {
                public void onReceivedError(WebView view, int errorCode, String description, String failingUrl) {
                    video.loadUrl("file:///android_asset/erro.html");
                }
            });
        } catch (Exception e) {
            System.out.println(e.toString());
        }
    }

    //create socket and socket connection to server and send the commands
    public boolean ButtonAction(String button) {
        CMD = button;
        final Socket_AsyncTask EnviaCMD;
        EnviaCMD   = new Socket_AsyncTask("192.168.0.6",8000,CMD);
        try {
            new Thread() {

                @Override
                public void run() {
                    EnviaCMD.execute();
                }
            }.start();

        } catch (Exception e) {
            Toast.makeText(getBaseContext(), "Erro: tente novamente" ,Toast.LENGTH_SHORT).show();
            return false;
        }
        return true;
    }

    //class create an object with thread and socket connection
    public class Socket_AsyncTask extends AsyncTask<Void,Void,Void> {
        String ip,commands;
        int port;
        public Socket_AsyncTask(String ip,int port,String commands){
            this.ip       = ip;
            this.port     = port;
            this.commands = commands;
        }
        @Override
        protected Void doInBackground(Void... params){
            try{
                InetAddress inetAddress = InetAddress.getByName(this.ip);
                socket = new java.net.Socket(inetAddress,this.port);

                if(socket.isConnected()) {
                    DataOutputStream dataOutputStream = new DataOutputStream(socket.getOutputStream());
                    dataOutputStream.writeBytes(this.commands);

                    dataOutputStream.close();
                   // socket           = null;
                   // dataOutputStream = null;
                }
                else{
                    Toast.makeText(getBaseContext(),"Connecting...",Toast.LENGTH_LONG).show();
                }

                socket.close();


            }catch (UnknownHostException e){ Toast.makeText(getBaseContext(), e.getMessage() , Toast.LENGTH_LONG);}catch (IOException e){ Toast.makeText(getBaseContext(), e.getMessage() , Toast.LENGTH_LONG);}catch (Exception e){e.printStackTrace();}
            return null;
        }
    }

   /* public class Socket_Async extends AsyncTask<Void,Void,Void> {
        @Override
        protected Void doInBackground(Void... params){
            try {
                InetAddress inetAddress = InetAddress.getByName(MainActivity.wifiModuleIp);
                socket = new java.net.Socket(inetAddress,MainActivity.wifiModulePort);

            } catch (SocketException e) {
                Toast.makeText(getBaseContext(), e.getMessage() , Toast.LENGTH_LONG);
            } catch (UnknownHostException e) {
                Toast.makeText(getBaseContext(), e.getMessage() , Toast.LENGTH_LONG);
            } catch (IOException e) {
                Toast.makeText(getBaseContext(), e.getMessage() , Toast.LENGTH_LONG);
            }
            return null;
        }
    }*/
}