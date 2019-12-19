package com.test.BTClient;
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

import com.test.BTClient.*;
import android.content.Context;
import android.os.Bundle;
import android.os.Vibrator;

public class factivity extends Activity{
//������Ϊ���صĽ���
	Button bt1,bt2,bt3,bt4,bt5,bt6,bt7,bt8,start,command,config,help;
	
	private TextView dis; 
	private InputStream is;    //������������������������
	
    boolean bRun = true;
    boolean bThread = false;
    private String fmsg = "";    //���������ݻ���
    private String mysmsg= ""; //������յ�������

	BTClient btc=new BTClient();
	
	  public void display(String str)
	    {
	    	Toast.makeText(this, str, Toast.LENGTH_SHORT).show();
	    }
	
	public void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_f);
		bt1=(Button)findViewById(R.id.Button01);
		 bt2=(Button)findViewById(R.id.Button02);
		bt3=(Button)findViewById(R.id.Button07);
		 bt4=(Button)findViewById(R.id.Button08);
		bt5=(Button)findViewById(R.id.Button09);
		bt6=(Button)findViewById(R.id.Button10);
		bt7=(Button)findViewById(R.id.Button11);
		 bt8=(Button)findViewById(R.id.Button12);
		
		start=(Button)findViewById(R.id.Button03);
		command=(Button)findViewById(R.id.Button04);
		config=(Button)findViewById(R.id.Button05);
		 help=(Button)findViewById(R.id.Button06);
		dis=(TextView)findViewById(R.id.Textview02);
//		sv = (ScrollView)findViewById(R.id.scrollView02);  //�õ���ҳ���
		bt1.setOnClickListener(new ButtonClickListener());
		bt2.setOnClickListener(new ButtonClickListener());
		bt3.setOnClickListener(new ButtonClickListener());
		bt4.setOnClickListener(new ButtonClickListener());
		bt5.setOnClickListener(new ButtonClickListener());
		bt6.setOnClickListener(new ButtonClickListener());
		bt7.setOnClickListener(new ButtonClickListener());
		bt8.setOnClickListener(new ButtonClickListener());
		command.setOnClickListener(new ButtonClickListener());
		start.setOnClickListener(new ButtonClickListener());
	}
	
	  //�˵�������
	   @Override
	    public boolean onCreateOptionsMenu(Menu menu) {//�����˵�
	        MenuInflater inflater = getMenuInflater();
	        inflater.inflate(R.menu.option_menu, menu);
	        return true;
	    }

	    @Override
	    public boolean onOptionsItemSelected(MenuItem item) { //�˵���Ӧ����
	        switch (item.getItemId()) {
	        case R.id.scan:
	        	if(btc._bluetooth.isEnabled()==false){
	        		Toast.makeText(this, "Open BT......", Toast.LENGTH_LONG).show();
	        		return true;
	        	}
	            // Launch the DeviceListActivity to see devices and do scan
	            Intent serverIntent = new Intent(this, DeviceListActivity.class);
	            startActivityForResult(serverIntent, btc.REQUEST_CONNECT_DEVICE);
	            return true;
	        case R.id.quit:
	            finish();
	            return true;
	        case R.id.clear:
	        	mysmsg="";
	        	dis.setText("");
	        	return true;
	        case R.id.save:
	        	btc.Save();
	        	return true;
	        }
	        return false;
	    }
	
	    //���ջ�������ӦstartActivityForResult()
	    public void onActivityResult(int requestCode, int resultCode, Intent data) {
	    	switch(requestCode){
	    	case BTClient.REQUEST_CONNECT_DEVICE:     //���ӽ������DeviceListActivity���÷���
	    		// ��Ӧ���ؽ��
	            if (resultCode == Activity.RESULT_OK) {   //���ӳɹ�����DeviceListActivity���÷���
	                // MAC��ַ����DeviceListActivity���÷���
	                String address = data.getExtras()
	                                     .getString(DeviceListActivity.EXTRA_DEVICE_ADDRESS);
	                // �õ������豸���      
	                btc._device = btc._bluetooth.getRemoteDevice(address);
	 
	                // �÷���ŵõ�socket
	                try{
	                	btc._socket = btc._device.createRfcommSocketToServiceRecord(UUID.fromString(btc.MY_UUID));
	                }catch(IOException e){
	                	Toast.makeText(this, "����ʧ�ܣ�", Toast.LENGTH_SHORT).show();
	                }
	                //����socket
	            //    MenuItem it = (MenuItem) findViewById(R.id.scan);
	                try{
	                	btc._socket.connect();
	                	Toast.makeText(this, "����"+btc._device.getName()+"�ɹ���", Toast.LENGTH_SHORT).show();
	             //   	it.setTitle("�Ͽ�");
	                }catch(IOException e){
	                	try{
	                		Toast.makeText(this, "����ʧ�ܣ�", Toast.LENGTH_SHORT).show();
	                		btc._socket.close();
	                		btc._socket = null;
	                	}catch(IOException ee){
	                		Toast.makeText(this, "����ʧ�ܣ�", Toast.LENGTH_SHORT).show();
	                	}
	                	
	                	return;
	                }
	            }
	            
                //�򿪽����߳�
                try{
            		is = btc._socket.getInputStream();   //�õ���������������
            		}catch(IOException e){
            			Toast.makeText(this, "��������ʧ�ܣ�", Toast.LENGTH_SHORT).show();
            			return;
            		}
            		if(bThread==false){
            			ReadThread.start();
            			bThread=true;
            		}else{
            			bRun = true;
            		}
    		break;
    	default:break;
    	}
	    }

//���������߳�
Thread ReadThread=new Thread(){
	public void run(){
		int num = 0;
		byte[] buffer = new byte[1024];
		byte[] buffer_new = new byte[1024];
		int i = 0;
		int n = 0;
		bRun = true;
		//�����߳�
		while(true){
			try{
				while(is.available()==0){
					while(bRun == false){}
				}
				while(true){
					num = is.read(buffer);         //��������
					n=0;
					
					String s0 = new String(buffer,0,num);
					fmsg+=s0;    //�����յ�����
					for(i=0;i<num;i++){
						if((buffer[i] == 0x0d)&&(buffer[i+1]==0x0a)){
							buffer_new[n] = 0x0a;
							i++;
						}else{
							buffer_new[n] = buffer[i];
						}
						n++;
					}
					String s = new String(buffer_new,0,n);
					if(s.equalsIgnoreCase("1")||s.equalsIgnoreCase("2") || s.equalsIgnoreCase("3") ||s.equalsIgnoreCase("4") ||s.equalsIgnoreCase("5") || s.equalsIgnoreCase("6") ||s.equalsIgnoreCase("7") ||s.equalsIgnoreCase("8"))
					{
						s="";
					}
					mysmsg+=s;   //д����ջ���
					
					if(s.equalsIgnoreCase("r")||s.equalsIgnoreCase("running!\n\n")||s.equalsIgnoreCase("unning!\n\n")||s.equalsIgnoreCase("running!\n")||s.equalsIgnoreCase("unning!\n"))
					{
						new VibratorDemoActivity().send(true);
						s="";
					}
					else if(s.equalsIgnoreCase("q")||s.equalsIgnoreCase("quit!\n\n")||s.equalsIgnoreCase("uit!\n\n")||s.equalsIgnoreCase("quit!\n")||s.equalsIgnoreCase("uit!\n\n"))
					{
					  new VibratorDemoActivity().send(false);
					  s="";
					}
					else s="";
					if(is.available()==0)break;  //��ʱ��û�����ݲ�����������ʾ
				}
				//������ʾ��Ϣ��������ʾˢ��
					handler.sendMessage(handler.obtainMessage());       	  //�������������⣬���»���ҳ���˳�  		
	    		}catch(IOException e){
	    		}
		}
	}
};
//��Ϣ�������
Handler handler= new Handler(){
	public void handleMessage(Message msg){
		super.handleMessage(msg);
		dis.setText(mysmsg);   //��ʾ���� 
	   dis.setMovementMethod(ScrollingMovementMethod.getInstance()) ;
//		sv.scrollTo(0,dis.getMeasuredHeight()); //�����������һҳ
	}
};

	class ButtonClickListener implements OnClickListener{
		public void onClick(View arg0){
			//todo
			if(arg0.equals(command))
			{
				Intent intent =new Intent();
				//����Ҫ�����Ľ���
				intent.setClass(factivity.this,BTClient.class);
				//��bundle���󴫵�����
			//	Bundle be=new Bundle();
				factivity.this.startActivity(intent);
				factivity.this.finish();
			}
			else if(arg0.equals(start))
			{
				display("�������������");
			}
			else if(arg0.equals(bt1))
			{
				byte[] tmp={49};
				btc.senddata(tmp);//����1
				display("�򿪵�");
			}
			else if(arg0.equals(bt2))
			{
				byte[] tmp={50};
				btc.senddata(tmp);//����2
				display("�رյ�");
			}
			else if(arg0.equals(bt3))
			{
				byte[] tmp={51};
				btc.senddata(tmp);//����3
				display("������");
			}
			else if(arg0.equals(bt4))
			{
				byte[] tmp={52};
				btc.senddata(tmp);//����4
				display("�ر�����");
			}
			else if(arg0.equals(bt5))
			{
				byte[] tmp={53};
				btc.senddata(tmp);//����5
				display("�������");
			}
			else if(arg0.equals(bt6))
			{
				byte[] tmp={54};
				btc.senddata(tmp);//����6
				display("ֹͣ���");
			}
			else if(arg0.equals(bt7))
			{
				byte[] tmp={55};
				btc.senddata(tmp);//����7
			//	display("��ȡϵͳʱ��");	
			}
			else if(arg0.equals(bt8))
			{
				byte[] tmp={56};
				btc.senddata(tmp);//����8
			//	display("��ȡ��ʪ��");
			}
		}
	}
	
	public class VibratorDemoActivity
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
		//	 long [] pattern = {100,400,100,400}; // ֹͣ ���� ֹͣ ����
		//	 vibrator.vibrate(pattern,2); //�ظ����������pattern ���ֻ����һ�Σ�index��Ϊ-1
		//	 Thread ctlThread;
		     try
			 {if (audioManager.getRingerMode() == AudioManager.RINGER_MODE_SILENT) //���ɾ�����ǿ���˳�
		      {
		         display("�Ѿ����ɾ���");
		         return;
		      } }
		      catch(Exception e){};
		      if(b)
		      {
			        long [] pattern = {100,400,100,400}; // ֹͣ ���� ֹͣ ����
		     	     vibrator.vibrate(pattern,2); //�ظ����������pattern ���ֻ����һ�Σ�index��Ϊ-1
		              r.play();  
		      }
		      else
		      {
		    	  vibrator.cancel();//ȡ����
		    	  if(r.isPlaying())
		    		  r.stop();
		      }
              
		      //��һ���߳�
		   /*    ctlThread = new Thread() {
		             public void run() {
		            	 try{
		            		 long [] pattern = {100,400,100,400}; // ֹͣ ���� ֹͣ ����
		            	     vibrator.vibrate(pattern,2); //�ظ����������pattern ���ֻ����һ�Σ�index��Ϊ-1
		            	     
		            	     Uri notification = RingtoneManager.getDefaultUri(RingtoneManager.TYPE_RINGTONE); 
		                     Ringtone r = RingtoneManager.getRingtone(getApplicationContext(), notification);  
		                     r.play();  
		            	 }
		            	 catch(Exception e){
		            		 
		            	 }
		             }
		      };*/
		 }	 
		 public void onStop()
		 {
			 
		 }
	}

}
