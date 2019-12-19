package com.test.BTClient;



import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.UUID;

import com.test.BTClient.DeviceListActivity;
import com.test.BTClient.factivity.ButtonClickListener;

import android.annotation.SuppressLint;
import android.annotation.TargetApi;
import android.app.Activity;
import android.app.AlertDialog;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.DialogInterface;
import android.content.Intent;
import android.os.Build;
import android.os.Bundle;
import android.os.Environment;
import android.os.Handler;
import android.os.Message;
import android.view.LayoutInflater;
import android.view.Menu;            //��ʹ�ò˵����������
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ScrollView;
import android.widget.TextView;
import android.widget.Toast;

@TargetApi(Build.VERSION_CODES.ECLAIR) @SuppressLint("NewApi") 
public class BTClient extends Activity {
	
	public final static int REQUEST_CONNECT_DEVICE = 1;    //�궨���ѯ�豸���
	
	public final static String MY_UUID = "00001101-0000-1000-8000-00805F9B34FB";   //SPP����UUID��
	
	private InputStream is;    //������������������������
	//private TextView text0;    //��ʾ������
    private EditText edit0;    //��������������
    private TextView dis;       //����������ʾ���
    private ScrollView sv;      //��ҳ���
    public String smsg = "";    //��ʾ�����ݻ���
    private String fmsg = "";    //���������ݻ���
    private String ctr ="";    //���������
    public static boolean isend=false;
    public static String s="";

    public String filename=""; //��������洢���ļ���
    public BluetoothDevice _device = null;     //�����豸
    public BluetoothSocket _socket = null;      //����ͨ��socket
    boolean _discoveryFinished = false;    
    boolean bRun = true;
    boolean bThread = false;
	
    public BluetoothAdapter _bluetooth = BluetoothAdapter.getDefaultAdapter();    //��ȡ�����������������������豸
	
    
    private Button start,command;
	
    public void display(String str)
    {
    	Toast.makeText(this, str, Toast.LENGTH_SHORT).show();
    }
    
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);   //���û���Ϊ������ main.xml
     //   text0 = (TextView)findViewById(R.id.Text0);  //�õ���ʾ�����
        edit0 = (EditText)findViewById(R.id.Edit0);   //�õ��������
        sv = (ScrollView)findViewById(R.id.ScrollView01);  //�õ���ҳ���
        dis = (TextView) findViewById(R.id.in);      //�õ�������ʾ���

       //����򿪱��������豸���ɹ�����ʾ��Ϣ����������
        if (_bluetooth == null){
        	Toast.makeText(this, "�޷����ֻ���������ȷ���ֻ��Ƿ����������ܣ�", Toast.LENGTH_LONG).show();
            finish();
            return;
        }
        
        // �����豸���Ա�����  
       new Thread(){
    	   public void run(){
    		   if(_bluetooth.isEnabled()==false){
        		_bluetooth.enable();
    		   }
    	   }   	   
       }.start();   
       
      start=(Button)findViewById(R.id.Button18);
      command=(Button)findViewById(R.id.Button19);
      start.setOnClickListener(new ButtonClickListener());
      command.setOnClickListener(new ButtonClickListener());
    }
    //button�ļ�����
    class ButtonClickListener implements OnClickListener{
    	public void onClick(View arg0){
    		//todo �л�ҳ��
    		if(arg0.equals(start))
    		{
	    		Intent intent =new Intent();
				//����Ҫ�����Ľ���
				intent.setClass(BTClient.this,factivity.class);
				//��bundle���󴫵�����
			//	Bundle be=new Bundle();
				BTClient.this.startActivity(intent);
				BTClient.this.onPause();
    		}
    		else if(arg0.equals(command))
    		{
    			display("�������������");
    		}
    	}
    }

    //���Ͱ�����Ӧ
    public void onSendButtonClicked(View v){
    	int i=0;
    	int n=0;
    	try{
    	  final OutputStream os = _socket.getOutputStream();   //�������������
    		/*byte[] bos = edit0.getText().toString().getBytes();
    		for(i=0;i<bos.length;i++){
    			if(bos[i]==0x0a)n++;
    		}
    		byte[] bos_new = new byte[bos.length+n];
    		n=0;
    		for(i=0;i<bos.length;i++){ //�ֻ��л���Ϊ0a,�����Ϊ0d 0a���ٷ���
    			if(bos[i]==0x0a){
    				bos_new[n]=0x0d;
    				n++;
    				bos_new[n]=0x0a;
    			}else{
    				bos_new[n]=bos[i];
    			}
    			n++;
    		}*/
    		byte[] bos = edit0.getText().toString().getBytes();
    		byte[] bos_new = new byte[bos.length+2];
    		for(i=0;i<bos.length;i++)
    		{
    			bos_new[i]=bos[i];
    			}
    		 //ĩβ����0d 0a���ٷ���
    		bos_new[bos.length]=0x0d;
    		bos_new[bos.length+1]=0x0a;
    		os.write(bos_new);	
    	}catch(IOException e){  		
    	}  	
    }
    
    //���ջ�������ӦstartActivityForResult()
    public void onActivityResult(int requestCode, int resultCode, Intent data) {
    	switch(requestCode){
    	case REQUEST_CONNECT_DEVICE:     //���ӽ������DeviceListActivity���÷���
    		// ��Ӧ���ؽ��
            if (resultCode == Activity.RESULT_OK) {   //���ӳɹ�����DeviceListActivity���÷���
                // MAC��ַ����DeviceListActivity���÷���
                String address = data.getExtras()
                                     .getString(DeviceListActivity.EXTRA_DEVICE_ADDRESS);
                // �õ������豸���      
                _device = _bluetooth.getRemoteDevice(address);
 
                // �÷���ŵõ�socket
                try{
                	_socket = _device.createRfcommSocketToServiceRecord(UUID.fromString(MY_UUID));
                }catch(IOException e){
                	Toast.makeText(this, "����ʧ�ܣ�", Toast.LENGTH_SHORT).show();
                }
                //����socket
            	Button btn = (Button) findViewById(R.id.Button13);
                try{
                	_socket.connect();
                	Toast.makeText(this, "����"+_device.getName()+"�ɹ���", Toast.LENGTH_SHORT).show();
                	btn.setText("�Ͽ�");
                }catch(IOException e){
                	try{
                		Toast.makeText(this, "����ʧ�ܣ�", Toast.LENGTH_SHORT).show();
                		_socket.close();
                		_socket = null;
                	}catch(IOException ee){
                		Toast.makeText(this, "����ʧ�ܣ�", Toast.LENGTH_SHORT).show();
                	}
                	
                	return;
                }
                
                //�򿪽����߳�
                try{
            		is = _socket.getInputStream();   //�õ���������������
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
            }
    		break;
    	default:break;
    	}
    }
    
    //���������߳�
    Thread ReadThread=new Thread(){
    	public void run(){
    		isend=false;
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
    					smsg+=s;   //д����ջ���
    					ctr=s;   //�����ָ�ֵ
    					if(is.available()==0) isend=true;s=smsg;break;  //��ʱ��û�����ݲ�����������ʾ
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
    		if(Trans(ctr,smsg)){}
    		else
    		dis.setText(smsg);   //��ʾ���� 
    		sv.scrollTo(0,dis.getMeasuredHeight()); //�����������һҳ
    	}
    };
    
    //�رճ�����ô�����
    public void onDestroy(){
    	super.onDestroy();
    	if(_socket!=null)  //�ر�����socket
    	try{
    		_socket.close();
    	}catch(IOException e){}
    //	_bluetooth.disable();  //�ر���������
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
        	if(_bluetooth.isEnabled()==false){
        		Toast.makeText(this, "Open BT......", Toast.LENGTH_LONG).show();
        		return true;
        	}
            // Launch the DeviceListActivity to see devices and do scan
            Intent serverIntent = new Intent(this, DeviceListActivity.class);
            startActivityForResult(serverIntent, REQUEST_CONNECT_DEVICE);
            return true;
        case R.id.quit:
            finish();
            return true;
        case R.id.clear:
        	smsg="";
        	dis.setText(smsg);
        	return true;
        case R.id.save:
        	Save();
        	return true;
        }
        return false;
    }
    
    //���Ӱ�����Ӧ����
    public void onConnectButtonClicked(View v){ 
    	if(_bluetooth.isEnabled()==false){  //����������񲻿�������ʾ
    		Toast.makeText(this, " ��������...", Toast.LENGTH_LONG).show();
    		return;
    	}
    	
    	
        //��δ�����豸���DeviceListActivity�����豸����
    	Button btn = (Button) findViewById(R.id.Button13);
    	if(_socket==null){
    		Intent serverIntent = new Intent(this, DeviceListActivity.class); //��ת��������
    		startActivityForResult(serverIntent, REQUEST_CONNECT_DEVICE);  //���÷��غ궨��
    	}
    	else{
    		 //�ر�����socket
    	    try{
    	    	
    	    	is.close();
    	    	_socket.close();
    	    	_socket = null;
    	    	bRun = false;
    	    	btn.setText("����");
    	    }catch(IOException e){}   
    	}
    	return;
    }
    
    //���水����Ӧ����
    public void onSaveButtonClicked(View v){
    	Save();
    }
    
    //���������Ӧ����
    public void onClearButtonClicked(View v){
    	smsg="";
    	fmsg="";
    	dis.setText(smsg);
    	return;
    }
    
    //�˳�������Ӧ����
    public void onQuitButtonClicked(View v){
    	finish();
    }
    
    //���湦��ʵ��
	public void Save() {
		//��ʾ�Ի��������ļ���
		LayoutInflater factory = LayoutInflater.from(BTClient.this);  //ͼ��ģ�����������
		final View DialogView =  factory.inflate(R.layout.sname, null);  //��sname.xmlģ��������ͼģ��
		new AlertDialog.Builder(BTClient.this)
								.setTitle("�ļ���")
								.setView(DialogView)   //������ͼģ��
								.setPositiveButton("ȷ��",
								new DialogInterface.OnClickListener() //ȷ��������Ӧ����
								{
									public void onClick(DialogInterface dialog, int whichButton){
										EditText text1 = (EditText)DialogView.findViewById(R.id.sname);  //�õ��ļ����������
										filename = text1.getText().toString();  //�õ��ļ���
										
										try{
											if(Environment.getExternalStorageState().equals(Environment.MEDIA_MOUNTED)){  //���SD����׼����
												
												filename =filename+".txt";   //���ļ���ĩβ����.txt										
												File sdCardDir = Environment.getExternalStorageDirectory();  //�õ�SD����Ŀ¼
												File BuildDir = new File(sdCardDir, "/data");   //��dataĿ¼���粻����������
												if(BuildDir.exists()==false)BuildDir.mkdirs();
												File saveFile =new File(BuildDir, filename);  //�½��ļ���������Ѵ������½��ĵ�
												FileOutputStream stream = new FileOutputStream(saveFile);  //���ļ�������
												stream.write(fmsg.getBytes());
												stream.close();
												Toast.makeText(BTClient.this, "�洢�ɹ���", Toast.LENGTH_SHORT).show();
											}else{
												Toast.makeText(BTClient.this, "û�д洢����", Toast.LENGTH_LONG).show();
											}
										
										}catch(IOException e){
											return;
										}
										
										
										
									}
								})
								.setNegativeButton("ȡ��",   //ȡ��������Ӧ����,ֱ���˳��Ի������κδ��� 
								new DialogInterface.OnClickListener() {
									public void onClick(DialogInterface dialog, int which) { 
									}
								}).show();  //��ʾ�Ի���
	} 

	//����������ִ���
	//��һ��str��Ҫ���ϵ��ַ������ڶ���str�Ǳ���Ҫ��ʾ���ַ���
	//����true��ʾ�����֣�����false��ʾ�ǿ�����
	private Boolean Trans(String ctr,String smsg)
	{
		if(ctr.equalsIgnoreCase("1"))
		{
			dis.setText(smsg+"\r\nLED���Ѵ�");   //��ʾ����
		}
		else if(ctr.equalsIgnoreCase("2"))
		{
			dis.setText(smsg+"\r\nLED���ѹر�");   //��ʾ����
		}
		else if(ctr.equalsIgnoreCase("3"))
		{
			dis.setText(smsg+"\r\n������");   //��ʾ����
		}
		else if(ctr.equalsIgnoreCase("4"))
		{
			dis.setText(smsg+"\r\n�����ر�");   //��ʾ����
		}
		else if(ctr.equalsIgnoreCase("5"))
		{
			dis.setText(smsg+"\r\n�������״̬");   //��ʾ����
		}
		else if(ctr.equalsIgnoreCase("6"))
		{
			dis.setText(smsg+"\r\n�رռ��״̬");   //��ʾ����
		}
		else if(ctr.equalsIgnoreCase("7"))
		{
			dis.setText(smsg+"\r\n��ȡϵͳʱ��");   //��ʾ����
		}
		else
		{
			ctr="";
			return false;
		//	ctr.setText(ctr);   //��ʾ����
		}
		ctr="";
		return true;
	}
	
	//ͨ��������������
	public boolean senddata(byte send[])
	{
		int i=0;
    	int n=0;
    	try{
    		OutputStream os = _socket.getOutputStream();   //�������������
    		byte[] send_new = new byte[send.length+2];
    		for(i=0;i<send.length;i++)
    		{
    			send_new[i]=send[i];
    		}
    		 //ĩβ����0d 0a���ٷ���
    		send_new[send.length]=0x0d;
    		send_new[send.length+1]=0x0a;
    		os.write(send_new);	
    	}catch(IOException e){  
    		return false;
    	}  	
		return true;
	}
	
	//ͨ��������������
	/*	public String getdata(){
			 //�򿪽����߳�
            try{
        		is = _socket.getInputStream();   //�õ���������������
        		}catch(IOException e){
        			Toast.makeText(this, "��������ʧ�ܣ�", Toast.LENGTH_SHORT).show();
        			return "no data";
        		}
        		if(bThread==false){
        			ReadThread.start();
        			bThread=true;
        		}else{
        			bRun = true;
        		}
        		
				return s;
		}*/
}