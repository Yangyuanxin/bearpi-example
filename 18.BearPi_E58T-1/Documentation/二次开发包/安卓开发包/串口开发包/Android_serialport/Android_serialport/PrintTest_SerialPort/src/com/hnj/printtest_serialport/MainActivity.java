package com.hnj.printtest_serialport;


import com.hnj.dp_nserialportlist.Serialport_Factory;
import com.hnj.printtest_serialport.util.SharedPreferencesUtil;
import com.hnj.printtest_serialport.util.Toast_Util;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.view.View;
import android.view.Window;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.TextView;

public class MainActivity extends Activity {

	private static String TAG = "MainActivity";
	private Button lable_bt,search_print_bt,receipt_bt,printlabe_bt,ptprint_bt,xiaozhun_bt;
	public static Serialport_Factory sf;
	private Handler mhandler;
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		this.requestWindowFeature(Window.FEATURE_NO_TITLE);
		setContentView(R.layout.fragment_main);
		mhandler=new MHandler();
		sf=Serialport_Factory.getSerialport_Factory(this,mhandler);
		SharedPreferencesUtil.initPreferences(this);
		search_print_bt=(Button)findViewById(R.id.search_print_bt);
		lable_bt=(Button)findViewById(R.id.lable_bt);
		printlabe_bt=(Button)findViewById(R.id.printlabe_bt);
		ptprint_bt=(Button)findViewById(R.id.ptprint_bt);
		receipt_bt=(Button)findViewById(R.id.receipt_bt);
		xiaozhun_bt=(Button)findViewById(R.id.xiaozhun_bt);
		
		lable_bt.setOnClickListener(new OnClickListener() {			
			@Override
			public void onClick(View arg0) {
				if(sf.isConnection())
				{					
					sf.setprintmode(1);		
				}else {
					Toast_Util.ToastString(getApplicationContext(), R.string.unconnected); //打印机已连接
				}				
				
			}
		});
		receipt_bt.setOnClickListener(new OnClickListener() {			
			@Override
			public void onClick(View arg0) {
				// TODO Auto-generated method stub
				if(sf.isConnection())
				{					
					sf.setprintmode(2);		
				}else {
					Toast_Util.ToastString(getApplicationContext(), R.string.unconnected); //打印机已连接
				}
			}
		});
		search_print_bt.setOnClickListener(new OnClickListener() {			
			@Override
			public void onClick(View arg0) {
				Intent intent=new Intent(MainActivity.this,SetActivity.class);
				startActivity(intent);
			}
		});
	printlabe_bt.setOnClickListener(new OnClickListener() {
			
			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				if(sf.isConnection())
				{					
					Intent intent=new Intent(MainActivity.this,Lable2PrintActivity.class);
					startActivity(intent);		
				}else {
					Toast_Util.ToastString(getApplicationContext(), R.string.unconnected); //打印机已连接
				}
			}
		});
	xiaozhun_bt.setOnClickListener(new OnClickListener() {		
		@Override
		public void onClick(View v) {
			// TODO Auto-generated method stub
			if(sf.isConnection())
			{					
				byte[] xz=new byte[]{0x1f,0x63};
				sf.Sendbyte(xz);
			}else {
				Toast_Util.ToastString(getApplicationContext(), R.string.unconnected); //打印机已连接
			}
		}
	});
	ptprint_bt.setOnClickListener(new OnClickListener() {			
			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				if(sf.isConnection())
				{					
					Intent intent=new Intent(MainActivity.this,NormalPrintActivity.class);
					startActivity(intent);		
				}else {
					Toast_Util.ToastString(getApplicationContext(), R.string.unconnected); //打印机已连接
				}
			}
		});
	}
	public static boolean ispage=false;
	@SuppressLint("HandlerLeak")
	 class MHandler extends Handler {
				@Override
				public void handleMessage(Message msg) {
					super.handleMessage(msg);			
					switch (msg.what) {						
						
						case Serialport_Factory.PAPER_STATE:{//检查缺纸状态
							boolean p=msg.getData().getBoolean("PAPER_STATE");
							if(p)
							{
								ispage=true;
								Toast_Util.ToastString(getApplicationContext(), "打印机有纸");
							}else {
								ispage=false;
								Toast_Util.ToastString(getApplicationContext(), "打印机缺纸");
							}
							break;
						}
					case Serialport_Factory.PRINTSTATE:{
						boolean printstate=msg.getData().getBoolean("printstate");
						if(printstate)
						{
							Toast_Util.ToastString(getApplicationContext(), "print completed");//打印完成
						}else
						{
							Toast_Util.ToastString(getApplicationContext(), "Print failure");//打印失败
						}
						 
						break;
					}
					}
				}
			}
	
}
