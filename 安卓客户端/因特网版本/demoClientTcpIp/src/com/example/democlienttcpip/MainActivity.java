package com.example.democlienttcpip;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.net.Socket;
import java.nio.charset.Charset;


import android.app.TabActivity;
import android.content.Context;
import android.content.Intent;
import android.media.AudioManager;
import android.media.Ringtone;
import android.media.RingtoneManager;
import android.net.Uri;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.os.Vibrator;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.EditText;
import android.widget.TabHost;
import android.widget.TextView;
import android.widget.Toast;

import android.os.Vibrator;

import android.media.AudioManager;
import android.media.Ringtone;
import android.media.RingtoneManager;

import java.io.IOException;
import java.io.InputStream;
import java.util.UUID;

import android.app.Activity;
import android.content.ClipData.Item;
import android.content.Intent;
import android.media.AudioManager;
import android.media.Ringtone;
import android.media.RingtoneManager;
import android.net.Uri;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.text.method.ScrollingMovementMethod;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ScrollView;
import android.widget.TextView;
import android.widget.Toast;

import android.content.Context;
import android.os.Bundle;
import android.os.Vibrator;

public class MainActivity<Params> extends TabActivity implements
		OnClickListener {

	private EditText edtIP;
	private EditText edtPort;
	EditText edtSend;
	private EditText edtReceiver;
	private TextView tvReceiver;
	
	private Button btnConn;
	private Button btnSend;

	private Button b1,b2,b3,b4,b5,b6,b7,b8;
	
//	private CheckBox checkBoxTimer;

	private String tag = "MainActivity";

	InputStream in;
	PrintWriter printWriter = null;
	BufferedReader reader;

	Socket mSocket = null;
	public boolean isConnected = false;

	private MyHandler myHandler;

	Thread receiverThread;
	
	Thread play;
	
	CheckBoxListener listener;

	//�������е����ݶ�ȡ�������µ�UI��,���Ӧ����poll���ƣ�һֱ��ɨ��������
	private class MyReceiverRunnable implements Runnable {

		public void run() {

			while (true) {

				Log.i(tag, "---->>client receive....");
				if (isConnected) {
					if (mSocket != null && mSocket.isConnected()) {

						String result = readFromInputStream(in);//��ȡ�������е�����

						try {
							if (!result.equals("")) {

								Message msg = new Message();//���յ����ݣ��߳̽���
								msg.what = 1;
								Bundle data = new Bundle();
								data.putString("msg", result);
								msg.setData(data);
								myHandler.sendMessage(msg);//�����͵�handle��
							}

						} catch (Exception e) {
							Log.e(tag, "--->>read failure!" + e.toString());
						}
					}
				}
				try {
					Thread.sleep(100L);
				} catch (InterruptedException e) {
					e.printStackTrace();
				}

			}
		}
	}

	private class MyHandler extends Handler {
		
		VibratorDemoActivity vd=new VibratorDemoActivity();
		@Override
		public void handleMessage(Message msg) {
			super.handleMessage(msg);

			receiverData(msg.what);
			if (msg.what == 1) {
				String result = msg.getData().get("msg").toString();//�����߳�handle�н�������
				tvReceiver.append(result);//���ݸ��µ�UI��
				//�ж�Ҫ��Ҫ�ֻ���������
				if(result.equalsIgnoreCase("r")||result.equalsIgnoreCase("running!\r\n\r\n\r\n")||result.equalsIgnoreCase("unning!\n\n")||result.equalsIgnoreCase("running!\n")||result.equalsIgnoreCase("unning!\n"))
				{
					//new VibratorDemoActivity().send(true);
					vd.send(true);
				}
				else if(result.equalsIgnoreCase("q")||result.equalsIgnoreCase("quit!\r\n\r\n\r\n")||result.equalsIgnoreCase("uit!\n\n")||result.equalsIgnoreCase("quit!\n")||result.equalsIgnoreCase("uit!\n\n"))
				{
				//	new VibratorDemoActivity().onStop();
					vd.send(false);
				}
			}
		}
	}

	@Override
	protected void onCreate(Bundle savedInstanceState) {//��������
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		init();
	}

		//���水ť���¼��ĳ�ʼ��
	private void init() {

		//��ȡip�Ͷ˿ڣ���������/���͵İ�ť����
		edtIP = (EditText) this.findViewById(R.id.id_edt_inputIP);
		edtPort = (EditText) this.findViewById(R.id.id_edt_inputport);
		btnSend=(Button) findViewById(R.id.id_btn_send);
		btnSend.setOnClickListener(this);
		btnConn = (Button) findViewById(R.id.id_btn_connClose);
		btnConn.setOnClickListener(this);
		
		b1 = (Button) findViewById(R.id.button1);
		b1.setOnClickListener(this);
		b2 = (Button) findViewById(R.id.button2);
		b2.setOnClickListener(this);
		b3 = (Button) findViewById(R.id.button3);
		b3.setOnClickListener(this);
		b4 = (Button) findViewById(R.id.button4);
		b4.setOnClickListener(this);
		b5 = (Button) findViewById(R.id.button5);
		b5.setOnClickListener(this);
		b6 = (Button) findViewById(R.id.button6);
		b6.setOnClickListener(this);
		b7 = (Button) findViewById(R.id.button7);
		b7.setOnClickListener(this);
		b8 = (Button) findViewById(R.id.button8);
		b8.setOnClickListener(this);
		
	//	edtReceiver=(EditText) this.findViewById(R.id.edtReceiver);
		tvReceiver=(TextView) this.findViewById(R.id.textView2);
		myHandler = new MyHandler();  //�½�һ��handler������Ϣ�����а����˽�����������һ��������·
		play=new Thread(new VibratorDemoActivity());
	}

	//
	/******************************************************************************/
	public String readFromInputStream(InputStream in) {
		int count = 0;
		byte[] inDatas = null;
		try {
			while (count == 0) {
				count = in.available();//��ȡ�������пɶ��ĳ���
			}
			inDatas = new byte[count];
			in.read(inDatas);        //�����������ݷŽ�����
			return new String(inDatas, "gb2312");  //��������
		} catch (Exception e) {
			e.printStackTrace();
		}
		return null;
	}

	/******************************************************************************/

	@Override
	public void onClick(View v) {

		try
		{
			switch (v.getId()) {

			// ����2�������߳�:���͡����ա�

			case R.id.id_btn_connClose:  //���Ӱ�ť

				connectThread();
				break;
			case R.id.id_btn_send:   //�������ݰ�ť

				sendData("hello!,I'm zq!");
				break;
			case R.id.button1:
				sendData("1");
				break;
				
			case R.id.button2:
				sendData("2");
				break;
			case R.id.button3:
				sendData("3");
				break;
			case R.id.button4:
				sendData("4");
				break;
			case R.id.button5:
				sendData("5");
				break;
			case R.id.button6:
				sendData("6");
				break;
			case R.id.button7:
				sendData("7");
				break;
			case R.id.button8:
				sendData("8");
				break;
			}
		}
		catch (Exception e)
		{
		  showInfo(e.toString());
		}
	}

	/**
	 * �����ӵ�������ʱ,���Դ��������¼�.
	 */
	private void receiverData(int flag) {

		if (flag == 2) {
			// mTask = new ReceiverTask();
			receiverThread = new Thread(new MyReceiverRunnable());//����һ������UI���߳�
			receiverThread.start();

			Log.i(tag, "--->>socket ���ӳɹ�!");
			btnConn.setText("�Ͽ�");

			isConnected = true;
			// mTask.execute(null);
		}

	}

	/**
	 * ���������߳�.
	 */
	private void sendData(String s) {

		// sendThread.start();
		try {
	//		String context = edtSend.getText().toString();//�ӱ༭���ȡҪ���͵�����
			String context = s;
			if (printWriter == null || context == null) {

				if (printWriter == null) {
					showInfo("����ʧ��!");
					return;
				}
				if (context == null) {
					showInfo("����ʧ��!");
					return;
				}
			}

			printWriter.print(context);//�����������
			printWriter.flush();
			Log.i(tag, "--->> client send data!");
		} catch (Exception e) {
			Log.e(tag, "--->> send failure!" + e.toString());

		}
	}

	/**
	 * ���������߳�.
	 *///�������ӳɹ��󣬿�����һ�������������߳�?
	private void connectThread() {
		if (!isConnected) {//δ����ʱ��
			new Thread(new Runnable() {

				@Override
				public void run() {
					Looper.prepare();//�½�һ���߳���Ϣ���ݻ���
					Log.i(tag, "---->> connect/close server!");

					connectServer(edtIP.getText().toString(), edtPort.getText()
							.toString());//����������ip�Ͷ˿ں�
				}
			}).start();
		} else {//�Ѿ�����ʱ
			try {
				if (mSocket != null) {
					mSocket.close();//�ر�����
					mSocket = null;
					Log.i(tag, "--->>ȡ��server.");
					// receiverThread.interrupt();
				}
			} catch (IOException e) {
				e.printStackTrace();
			}
			btnConn.setText("����");
			isConnected = false;
		}
	}

	// ���ӷ�����.(����������ֵķ������˱��뷽ʽ:gb2312)
	private void connectServer(String ip, String port) {
		try {
			Log.e(tag, "--->>start connect  server !" + ip + "," + port);

			mSocket = new Socket(ip, Integer.parseInt(port));//��ȡһ�������׽���
			Log.e(tag, "--->>end connect  server!");

			OutputStream outputStream = mSocket.getOutputStream();//���׽��ֻ�ȡһ�������

			printWriter = new PrintWriter(new BufferedWriter(
					new OutputStreamWriter(outputStream,
							Charset.forName("gb2312"))));//ת��������ʽ
		//	listener.setOutStream(printWriter); //��ȡ�����  ��������
			// reader = new BufferedReader(new InputStreamReader(
			// mSocket.getInputStream()));

			in = mSocket.getInputStream();  //���׽��ֻ���һ��������
			myHandler.sendEmptyMessage(2);  //���ӳɹ��������߳���Ϣ�ط�����Ϣ���Ὺ��һ��ɨ�����������߳�

			showInfo("���ӳɹ�!");
		} catch (Exception e) {
			isConnected = false;
			showInfo("����ʧ�ܣ�");
			Log.e(tag, "exception:" + e.toString());
		}

	}

	private void showInfo(String msg) {
		Toast.makeText(MainActivity.this, msg, Toast.LENGTH_SHORT).show();

	}

    @Override
    
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.main, menu);
        return true;
    }
    
    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        switch (item.getItemId()) 
        {
        	case R.id.action_settings:
        		finish();
        		return true;
        	case R.id.item1:
        		tvReceiver.setText("");
        }
        return true;
    }
    
    class VibratorDemoActivity implements Runnable
    {
    	protected AudioManager audioManager;
    	 protected Vibrator vibrator;
    	 protected  Ringtone r;

    	 public VibratorDemoActivity()
    	 {
    		 /** �������𶯴�С����ͨ���ı�pattern���趨���������ʱ��̫�̣���Ч�����ܸо�����* */
    		vibrator = (Vibrator)getSystemService(Context.VIBRATOR_SERVICE);
    		audioManager = (AudioManager)getSystemService(Context.AUDIO_SERVICE); //�˷�������Context���õ�
    	    Uri notification = RingtoneManager.getDefaultUri(RingtoneManager.TYPE_RINGTONE); 
            r = RingtoneManager.getRingtone(getApplicationContext(), notification);  
    	 }
    	 public void send(boolean b)
    	 {
    	     try
    		 {if (audioManager.getRingerMode() == AudioManager.RINGER_MODE_SILENT) //���ɾ�����ǿ���˳�
    	      {
    			 showInfo("�Ѿ����ɾ���");
    	         return;
    	      } }
    	      catch(Exception e){};
    	      if(b)
    	      {
    		        long [] pattern = {100,400,100,400}; // ֹͣ ���� ֹͣ ����
    	     	     vibrator.vibrate(pattern,2); //�ظ����������pattern ���ֻ����һ�Σ�index��Ϊ-1  ����ͣ��
    	              r.play(); 
    	      }
    	      else
    	      {
    	    	  vibrator.cancel();//ȡ����
    	    	  if(r.isPlaying())
    	    		  r.stop();
    	      }
              
    	 }	 
    	 public void onStop()
    	 {
    		  vibrator.cancel();//ȡ����
	    	  if(r.isPlaying())
	    		  r.stop();
    	 }
		@Override
		public void run() {
			// TODO Auto-generated method stub
			
		}
    }
	
}

