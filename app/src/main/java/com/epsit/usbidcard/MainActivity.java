package com.epsit.usbidcard;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Handler;
import android.os.Message;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.RelativeLayout;
import android.widget.TextView;
import android.widget.Toast;

import com.cmcc.nativepackage.UsbIDCard;

import java.io.FileInputStream;
import java.io.FileNotFoundException;

public class MainActivity extends AppCompatActivity implements View.OnClickListener {


    RelativeLayout relativeLayout2;

    private ImageView imageView1;
    private ImageView imageView2;
    private ImageView imageView3;

    private Button searchBtn;
    private Button checkOTGBtn;
    private Button getPermissionBtn;

    private TextView text1;
    private TextView text2;
    private TextView text3;
    private TextView text4;
    private TextView  textView4_1;
    private TextView text5;
    private TextView text6;
    private TextView text7;
    private TextView text8;

    String TAG = "IDCard";

    private final int DEVICE_CONNECT_SUCCESS = 101;
    private final int DEVICE_CONNECT_FALSE = 102;

    private final int PERMISSION_SUCCESS = 201;
    private final int PERMISSION_FALSE = 202;

    private final int OPEN_DEVICE_SUCCESS = 301;
    private final int OPEN_DEVICE_FALSE = 302;

    private final int READ_DEVICE_SUCCESS = 401;
    private final int READ_DEVICE_FALSE = 402;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_idcard2);

        imageView1 = (ImageView) findViewById(R.id.imageView1);
        imageView2 = (ImageView) findViewById(R.id.imageView2);
        imageView3 = (ImageView) findViewById(R.id.imageView3);
        searchBtn = (Button) findViewById(R.id.button3);
        checkOTGBtn = (Button) findViewById(R.id.button1);
        getPermissionBtn = (Button) findViewById(R.id.button2);

        text1 = (TextView) findViewById(R.id.textView1);
        text2 = (TextView) findViewById(R.id.textView2);
        text3 = (TextView) findViewById(R.id.textView3);
        text4 = (TextView) findViewById(R.id.textView4);
        textView4_1 = (TextView) findViewById(R.id.textView4_1);
        text5 = (TextView) findViewById(R.id.textView5);
        text6 = (TextView) findViewById(R.id.textView6);
        text7 = (TextView) findViewById(R.id.textView7);
        text8 = (TextView) findViewById(R.id.textView8);

        // relativeLayout2 = (RelativeLayout)
        // findViewById(R.id.relativeLayout2);
        // relativeLayout2.setVisibility(View.INVISIBLE)

        text1.setVisibility(View.INVISIBLE);
        text2.setVisibility(View.INVISIBLE);
        text3.setVisibility(View.INVISIBLE);
        text4.setVisibility(View.INVISIBLE);
        text5.setVisibility(View.INVISIBLE);
        text6.setVisibility(View.INVISIBLE);
        text7.setVisibility(View.INVISIBLE);
        text8.setVisibility(View.INVISIBLE);
        imageView1.setVisibility(View.INVISIBLE);
        imageView2.setVisibility(View.INVISIBLE);
        imageView3.setVisibility(View.INVISIBLE);
        UsbIDCard.setContext(this);
        checkOTGBtn.setOnClickListener(new View.OnClickListener() {

            @Override
            public void onClick(View v) {
                // TODO Auto-generated method stub
                int isOTG = UsbIDCard.isOTGDevice();
                Log.d(TAG, "OTG : "+ (isOTG == 0 ? "True" : "False"));
                if(isOTG == 0)
                {
                    Log.d(TAG, "get OTGDevice success.");
                    handler.sendEmptyMessage(DEVICE_CONNECT_SUCCESS);
                }else {
                    Log.d(TAG, "get OTGDevice  false.");
                    handler.sendEmptyMessage(DEVICE_CONNECT_FALSE);
                }
            }
        });

        getPermissionBtn.setOnClickListener(new View.OnClickListener() {

            @Override
            public void onClick(View v) {
                // TODO Auto-generated method stub

                new Thread(new Runnable() {

                    @Override
                    public void run() {
                        // TODO Auto-generated method stub
                        int per = UsbIDCard.getDevicePermission();
                        if(per == 0)
                        {
                            Log.d(TAG, "get Device Permission success.");
                            handler.sendEmptyMessage(PERMISSION_SUCCESS);
                        }else {
                            Log.d(TAG, "get Device Permission false.");
                            handler.sendEmptyMessage(PERMISSION_FALSE);
                        }
                    }
                }).start();

            }
        });

        //点击获取二代证信息
        searchBtn.setOnClickListener(new View.OnClickListener() {

            @Override
            public void onClick(View v) {
                text1.setText("");
                text2.setText("");
                text3.setText("");
                text4.setText("");
                text5.setText("");
                text6.setText("");
                text7.setText("");
                text8.setText("");
                imageView1.setVisibility(View.INVISIBLE);
                imageView2.setVisibility(View.INVISIBLE);
                imageView3.setVisibility(View.INVISIBLE);
                new Thread(new Runnable() {

                    @Override
                    public void run() {
                        // TODO Auto-generated method stub
                        int ret = UsbIDCard.openIDCard(1, "", "");

                        if(ret == 0)
                        {

                            ret = UsbIDCard.initialIDCard();

                            String[] info = new String[10];
                            byte[] img = new byte[200 * 1000];
                            ret = UsbIDCard.getIdCardInfo(info, img);

                            if (ret == 0) {
                                String localImg = info[8].trim();
                                Bitmap bitmap = getLoacalBitmap(localImg);

                                Message msg = new Message();
                                msg.what = READ_DEVICE_SUCCESS;
                                Bundle bundle = new Bundle();
                                bundle.putStringArray("info", info);
                                msg.setData(bundle);
                                msg.obj = bitmap;

                                handler.sendMessage(msg);
                            } else {
                                Message msg = new Message();
                                msg.what = READ_DEVICE_FALSE;
                                msg.arg1 = ret;
                                handler.sendMessage(msg);
                            }
                            UsbIDCard.closeIDCard();
                        }else{
                            handler.sendEmptyMessage(OPEN_DEVICE_FALSE);
                        }
                    }
                }).start();

            }

        });


    }

    private Handler handler = new Handler() {

        public void handleMessage(android.os.Message msg) {
            switch (msg.what) {
                case DEVICE_CONNECT_SUCCESS:
                    Toast.makeText(getApplicationContext(),
                            "已有设备连接到手机", Toast.LENGTH_SHORT).show();
                    break;
                case DEVICE_CONNECT_FALSE:
                    Toast.makeText(getApplicationContext(),
                            "没有设备连接到手机", Toast.LENGTH_SHORT).show();
                    break;
                case PERMISSION_SUCCESS:
                    Toast.makeText(getApplicationContext(),
                            "获取USB权限成功", Toast.LENGTH_SHORT).show();
                    break;
                case PERMISSION_FALSE:
                    Toast.makeText(getApplicationContext(),
                            "获取USB权限失败", Toast.LENGTH_SHORT).show();
                    break;


                case OPEN_DEVICE_SUCCESS:
                    Toast.makeText(getApplicationContext(),
                            "打开USB二代证成功", Toast.LENGTH_SHORT).show();

                    break;

                case OPEN_DEVICE_FALSE:
                    Toast.makeText(getApplicationContext(),
                            "打开USB二代证失败", Toast.LENGTH_SHORT).show();
                    break;


                case READ_DEVICE_SUCCESS:
                    Toast.makeText(getApplicationContext(),
                            "读取USB二代证成功", Toast.LENGTH_SHORT).show();

                    Bundle bundle = msg.getData();
                    String []info = bundle.getStringArray("info");
                    String name = info[0].trim();
                    Bitmap bitmap = (Bitmap)msg.obj;

                    text1.setText("姓名 : " + name);
                    text2.setText("性别 : " + info[1]);
                    text8.setText("民族 : " + info[2]);
                    text3.setText("出生日期 : " + info[3]);
                    text4.setText("" + info[4]);
                    text5.setText("身份证号 : " + info[5]);
                    text6.setText("签发机关 : " + info[6]);
                    text7.setText("有效期 : " + info[7]);

                    imageView3.setImageBitmap(bitmap);

                    textView4_1.setVisibility(View.VISIBLE);
                    text1.setVisibility(View.VISIBLE);
                    text2.setVisibility(View.VISIBLE);
                    text3.setVisibility(View.VISIBLE);
                    text4.setVisibility(View.VISIBLE);
                    text5.setVisibility(View.VISIBLE);
                    text6.setVisibility(View.VISIBLE);
                    text7.setVisibility(View.VISIBLE);
                    text8.setVisibility(View.VISIBLE);
                    imageView1.setVisibility(View.VISIBLE);
                    imageView2.setVisibility(View.VISIBLE);
                    imageView3.setVisibility(View.VISIBLE);
                    break;

                case READ_DEVICE_FALSE:
                    Toast.makeText(getApplicationContext(),
                            "读取USB二代证失败 : "+msg.arg1, Toast.LENGTH_SHORT).show();


                    break;
            }

        };
    };


    boolean isThreadRun = false;
    Thread mThread = new Thread(new Runnable() {

        @Override
        public void run() {
            // TODO Auto-generated method stub
            isThreadRun = true;

        }
    });


    public static Bitmap getLoacalBitmap(String url) {
        try {
            FileInputStream fis = new FileInputStream(url);
            return BitmapFactory.decodeStream(fis);
        } catch (FileNotFoundException e) {
            e.printStackTrace();
            return null;
        }
    }

    @Override
    protected void onRestart() {
        // TODO Auto-generated method stub
        super.onRestart();
        UsbIDCard.setContext(this);
    }

    @Override
    protected void onResume() {
        // TODO Auto-generated method stub
        super.onResume();
        UsbIDCard.setContext(this);
    }


    @Override
    protected void  onStop(){
        super.onStop();
        UsbIDCard.unregisterR();
    }


    @Override
    public void onClick(View v) {

    }
}
