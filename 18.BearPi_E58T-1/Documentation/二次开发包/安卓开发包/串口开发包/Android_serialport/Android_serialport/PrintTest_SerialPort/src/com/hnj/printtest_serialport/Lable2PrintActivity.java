package com.hnj.printtest_serialport;

import java.io.IOException;
import java.io.InputStream;

import com.hnj.printtest_serialport.util.Toast_Util;

import android.app.Activity;
import android.content.res.AssetManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.Window;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.TextView;

public class Lable2PrintActivity extends Activity {


	private Button bt_text,bt_barcode,bt_qr,bt_image,bt_cuttall,bt_cutting,bt_textimage,bt_printdemo1,bt_printdemo2;
	private TextView title_tv;
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		this.requestWindowFeature(Window.FEATURE_NO_TITLE);
		setContentView(R.layout.activity_normal);
		bt_text=(Button)findViewById(R.id.bt_text);
		title_tv=(TextView)findViewById(R.id.title_tv);
		bt_barcode=(Button)findViewById(R.id.bt_barcode);
		bt_qr=(Button)findViewById(R.id.bt_qr);
		bt_image=(Button)findViewById(R.id.bt_image);
		bt_cuttall=(Button)findViewById(R.id.bt_cuttall);
		bt_cutting=(Button)findViewById(R.id.bt_cutting);
		bt_textimage=(Button)findViewById(R.id.bt_textimage);
		bt_printdemo1=(Button)findViewById(R.id.bt_printdemo1);
		bt_printdemo2=(Button)findViewById(R.id.bt_printdemo2);
		title_tv.setText("两寸标签测试打印");
		//打印文本
		bt_text.setOnClickListener(new OnClickListener() {			
			@SuppressWarnings("static-access")
			@Override
			public void onClick(View arg0) {
//				if(!MainActivity.sf.Check_Paper())
//				{
//					Toast_Util.ToastString(getApplicationContext(),  R.string.printer_does_not_have_paper);
//					return;
//				}
				/**
				 * LableText 标签文本打印
				 * 第一个参数  X轴横向位移量
				 * 第二个参数  Y轴纵向位移量
				 * 第三个参数  字体大小 有效值1-6
				 * 第四个参数   是否旋转 0不旋转，1旋转90°
				 * 第五个参数   文本内容
				 */
				MainActivity.sf.LabelBegin(576, 550);//先设置标签的宽度和高度		
				MainActivity.sf.LableText(0, 0, 1, 0, "fdsfdsfsdfsd");
				MainActivity.sf.LableText(0, 30, 2, 0, "fdsfdsfsdfsd");
				MainActivity.sf.LableText(0, 80, 3, 0, "fdsfdsfsdfsd");
				MainActivity.sf.LableText(0, 120, 4, 0, "fdsfdsfsdfsd");
				MainActivity.sf.LableText(0, 180, 5, 0, "fdsfdsfsdfsd");
				MainActivity.sf.LableText(0, 360, 6, 0, "fdsfdsfsdfsd");
				MainActivity.sf.Labelend();//必须要有标签结束，否则不会打印
				MainActivity.sf.PaperCut();
			}
		});
		
//		打印条码
		bt_barcode.setOnClickListener(new OnClickListener() {			
			@SuppressWarnings("static-access")
			@Override
			public void onClick(View arg0) {
//				if(!MainActivity.sf.Check_Paper())
//				{
//					Toast_Util.ToastString(getApplicationContext(), R.string.printer_does_not_have_paper);
//					return;
//				}
				/**
				 * LableBarcode 标签条形码打印
				 * 第一个参数  X轴横向位移量
				 * 第二个参数  Y轴纵向位移量
				 * 第三个参数  条码高度 有效值10-200
				 * 第四个参数   条码线条宽度，1-4
				 * 第五个参数  是否旋转 0不旋转，1旋转90°
				 * 第六个参数   条码文本内容，不能包含有中文字符。
				 */
				Log.e("==========", "====="+System.currentTimeMillis());
				MainActivity.sf.LabelBegin(576, 320);//先设置标签的宽度和高度	
				MainActivity.sf.LableBarcode(0, 25, 42, 2, 0, "abfc-12345654645");
				MainActivity.sf.LableBarcode(0, 90,40, 3, 0, "654");
				MainActivity.sf.LableBarcode(0, 150,40, 4, 0, "654");
				MainActivity.sf.LableBarcode(0, 200,40, 1, 0, "654");
				MainActivity.sf.Labelend();//必须要有标签结束，否则不会打印
				MainActivity.sf.PaperCut();
				Log.e("==========", "=0000===="+System.currentTimeMillis());
			}
		});
		//打印二维码
		bt_qr.setOnClickListener(new OnClickListener() {			
			@Override
			public void onClick(View arg0) {
//				if(!MainActivity.sf.Check_Paper())
//				{
//					Toast_Util.ToastString(getApplicationContext(), R.string.printer_does_not_have_paper);
//					return;
//				}
				Log.e("========LabelQRCode===", "==222==");
//				 ==1A 31 00 05 04 32 00 05 00 05 00 E4 B8 AD E5 9B BD E6 B7 B1 E5 9C B3 00==
//						 ==1A 31 00 05 04 32 00 0A 00 06 00 61 62 63 31 32 33 00==

				/**
				 * LabelQRCode 标签二维码打印
				 * 第一个参数  X轴横向位移量
				 * 第二个参数  Y轴纵向位移量
				 * 第三个参数  二维码大小 有效值2-8
				 * 第四个参数   二维码文本内容
				 */
				MainActivity.sf.LabelBegin(384, 230);		//先设置标签的宽度和高度		56*30标签纸	
				MainActivity.sf.LabelQRCode(50, 5, 5,"中国深圳");
				MainActivity.sf.Labelend();//必须要有标签结束，否则不会打印	
//				MainActivity.sf.LabelBegin(384, 250);				
//				MainActivity.sf.LabelQRCode(50, 10, 6,"abc123");
//				MainActivity.sf.Labelend();//必须要有标签结束，否则不会打印	
				MainActivity.sf.PaperCut();
				
			}
		});
//		打印图片
		bt_image.setOnClickListener(new OnClickListener() {			
			@SuppressWarnings("static-access")
			@Override
			public void onClick(View arg0) {
//				if(!MainActivity.sf.Check_Paper())//检查是否有纸
//				{
//					Toast_Util.ToastString(getApplicationContext(), R.string.printer_does_not_have_paper);
//					return;
//				}
				/**
				 * LableImage 标签图片打印
				 * 第一个参数  X轴横向位移量
				 * 第二个参数  Y轴纵向位移量
				 * 第三个参数  图片大小最大范围不能超过标签纸的宽度，最大不能超过384,如果图片是800*400打印机会做等比例缩放处理到要打印图片的大小
				 * 第四个参数   bitmap图片
				 */
				Bitmap bm = getImageFromAssetsFile("1.jpg");
				if (null != bm) {
					
					MainActivity.sf.LabelBegin(384, 230);//先设置标签的宽度和高度	56*30标签纸
					MainActivity.sf.LableImage(50, 10, 280, bm);
					MainActivity.sf.Labelend();//必须要有标签结束，否则不会打印
					MainActivity.sf.PaperCut();
				}else {
					Toast_Util.ToastString(getApplicationContext(), R.string.no_printed_pictures);//没有打印图片
				}
			}
		});
		
		bt_textimage.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				/** LableTextImage 文本图片标签打印
				//第一个参数  X轴横向位移量
				//第二个参数  Y轴纵向位移量
				//第三个参数    打印图片宽度
				//第四个参数    字体大小
				//第五个参数    生成图片文本内容
				*/
				MainActivity.sf.LabelBegin(384, 235);//先设置标签的宽度和高度	56*30标签纸
				MainActivity.sf.LableTextImage(0, 0, 200, 30, "中国人民");
				MainActivity.sf.LableTextImage(0, 30, 200, 40,"中国人民");
				MainActivity.sf.LableTextImage(0, 80, 200, 50,"中国人民");
				MainActivity.sf.LableTextImage(0, 130, 384, 100,"中国人民");
				MainActivity.sf.Labelend();//必须要有标签结束，否则不会打印
				MainActivity.sf.PaperCut();
			}
		});
		bt_cutting.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				MainActivity.sf.PaperCut();//半切
			}
		});
		bt_cuttall.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				MainActivity.sf.PaperAllCut();//全切
			}
		});
		
		bt_printdemo1.setOnClickListener(new OnClickListener() {
			
			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
//				if(!MainActivity.sf.Check_Paper())
//				{
//					Toast_Util.ToastString(getApplicationContext(), R.string.printer_does_not_have_paper);
//					return;
//				}
				
				
//				MainActivity.sf.LabelBegin(384, 250);//先设置标签的宽度和高度	56*30标签纸
//				MainActivity.sf.LableText(0,5,2,0,"极速物流");
//				MainActivity.sf.LableText(295,5,2,0,"电子");
//				MainActivity.sf.LableText(10,140,2,0,"广州-->");
//				MainActivity.sf.LableText(295,140,2,0,"汽运");
//				MainActivity.sf.LableText(10,200,1,0,"厦门");				
//				MainActivity.sf.LableBarcode(20,65,65, 3, 0, "123456789586");
//				MainActivity.sf.LableText(265,200,1,0,"2018-08-18");
//				MainActivity.sf.Labelend();//必须要有标签结束，否则不会打印
//				MainActivity.sf.PaperCut();//半切
				
				testdemo1();
			}
		});
		bt_printdemo2.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
//				if(!MainActivity.sf.Check_Paper())
//				{
//					Toast_Util.ToastString(getApplicationContext(), R.string.printer_does_not_have_paper);
//					return;
//				}
				demo3();
			}
		});
		if(getactivit!=null)
		{
			getactivit.onbackactivity(this);
		}
	}
	private void testdemo()
	{
		Bitmap bm = getImageFromAssetsFile("e.jpg");
		MainActivity.sf.LabelBegin(384, 230);//先设置标签的宽度和高度	
//		MainActivity.sf.LabelQRCode(150, 10,4,"二维码内容");
		MainActivity.sf.LableImage(80, 10, 200, bm);
//		MainActivity.sf.LableText(100,150,1,0,"设备编号：*******xxxx");
//		MainActivity.sf.LableText(100,180,1,0,"住户编号：*******xxxx");
		MainActivity.sf.Labelend();//必须要有标签结束，否则不会打印
		MainActivity.sf.PaperAllCut();
	}
	private void testdemo1()
	{
		MainActivity.sf.PrintText("默认文字居左",1,1,20);		
		MainActivity.sf.PrintQR("爱我", 5,2);
		MainActivity.sf.GapCut();
	}
	private void demo3()
	{
		MainActivity.sf.LabelBegin(384, 230);//先设置标签的宽度和高度	56*30标签纸
		MainActivity.sf.LabelQRCode(0, 25,5,"电 池");
		MainActivity.sf.LableText(200,10,1,0,"MC 1001");
		MainActivity.sf.LableText(200,40,1,0,"电 池");
		MainActivity.sf.LableText(200,70,1,0,"2018-8-18");
		MainActivity.sf.LableText(200,100,1,0,"18:18:18");
		MainActivity.sf.LableText(200,130,1,0,"STCP00000369");				
		MainActivity.sf.LableText(200,160, 1, 0, "2018-8-28前有效");
		MainActivity.sf.LableText(200,190,1,0,"LS 32006");
		MainActivity.sf.Labelend();//必须要有标签结束，否则不会打印
		MainActivity.sf.PaperCut();
	}
	/**
	 * 打印测试页面
	 * @param v
	 */
	public void printtestpage(View v)
	{
		MainActivity.sf.PrintTestPage();
	}
	
	/**
	 * 检查缺纸
	 * 
	 */
	@SuppressWarnings("static-access")
	public void checkpage(View v)
	{
//		if(MainActivity.sf.Check_Paper())
//		{
//			Toast_Util.ToastString(getApplicationContext(),R.string.printer_have_paper);
//		}else{
//			Toast_Util.ToastString(getApplicationContext(), R.string.printer_does_not_have_paper);
//		}
//		String ts=MainActivity.sf.Check_Paper2();
//		Toast_Util.ToastString(getApplicationContext(),"=检查缺纸返回数据=="+ts);
		
		if(MainActivity.ispage)
		{
			Toast_Util.ToastString(getApplicationContext(), R.string.printer_have_paper);
		}else {
			Toast_Util.ToastString(getApplicationContext(), R.string.printer_does_not_have_paper);
		}
	}
	
	/**
	 * 从Assets中读取图片
	 */
	private Bitmap getImageFromAssetsFile(String fileName) {
		Bitmap image = null;
		AssetManager am = getResources().getAssets();
		try {
			InputStream is = am.open(fileName);
			image = BitmapFactory.decodeStream(is);
			is.close();
		} catch (IOException e) {
			e.printStackTrace();
		}

		return image;

	}
	
	public void back(View v)
	{
		finish();
	}
	
	
	private static onGetActivit getactivit = null;
	
	public static void setOnGetActivit(onGetActivit mactivit){
		getactivit = mactivit;
	}
	
	public interface onGetActivit {
	    void onbackactivity(Activity at);
	}
}
