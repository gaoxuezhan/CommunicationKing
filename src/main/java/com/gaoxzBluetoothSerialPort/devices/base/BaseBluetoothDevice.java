package com.gaoxzBluetoothSerialPort.devices.base;

import javax.microedition.io.Connector;
import javax.microedition.io.StreamConnection;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

public class BaseBluetoothDevice {
    protected String deviceAddress;
    protected StreamConnection streamConnection;
    protected InputStream inputStream;
    protected OutputStream outputStream;

    public BaseBluetoothDevice(String deviceAddress){
        this.deviceAddress = deviceAddress;
    }

    public void sayHi(){
        try {
            // deviceAddress = "98DA6007C740"; // 蓝牙设备的地址
            String url = "btspp://" + deviceAddress + ":1;authenticate=false;encrypt=false;master=false";
            this.streamConnection = (StreamConnection) Connector.open(url);
            this.inputStream = streamConnection.openInputStream();
            this.outputStream = streamConnection.openOutputStream();
        } catch (IOException ex) {
            ex.printStackTrace();
        }
    }
    public void sayGoodbye() {
        try {
            inputStream.close();
            streamConnection.close();
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }

    public InputStream getInputStream(){
        return this.inputStream;
    }

    public OutputStream getOutputStream(){
        return this.outputStream;
    }

    public void run(){
        while (true) {
            byte[] buffer = new byte[1024];
            int bytesRead = 0;
            try {
                bytesRead = inputStream.read(buffer);
            } catch (IOException e) {
                throw new RuntimeException(e);
            }

            String receivedMessage = new String(buffer, 0, bytesRead);
            System.out.println("Received message: " + receivedMessage);
        }
    }

}


