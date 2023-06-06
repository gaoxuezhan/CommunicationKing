package com.gaoxzBluetoothSerialPort.devices.base;

import com.fazecast.jSerialComm.SerialPort;
import com.fazecast.jSerialComm.SerialPortDataListener;
import com.fazecast.jSerialComm.SerialPortEvent;
import com.gaoxzBluetoothSerialPort.devices.data.SensorData;

import javax.microedition.io.Connector;
import javax.microedition.io.StreamConnection;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.Timer;
import java.util.TimerTask;

public class BaseUSBDevice {
    protected String desiredPortName;
    protected SerialPort port;
    protected SerialPortDataListener serialPortDataListener;

    public BaseUSBDevice(String desiredPortName){
        this.desiredPortName = desiredPortName;
        init();
    }
    public void init(){
        SerialPort selectedPort = null;
        SerialPort[] ports = SerialPort.getCommPorts();
        for (SerialPort port : ports) {
            if (port.getSystemPortName().equals(desiredPortName)) {
                selectedPort = port;
                break;
            }
        }

        if (selectedPort != null) {
            // 选择第一个串口
            port = selectedPort;

            // 打开串口
            if (port.openPort()) {
                // 设置串口参数（波特率、数据位、停止位等）
                port.setBaudRate(9600);
                port.setParity(SerialPort.NO_PARITY);
                port.setNumDataBits(8);
                port.setNumStopBits(SerialPort.ONE_STOP_BIT);
                this.setSerialPortDataListener(this.serialPortDataListener);

            } else {
                System.out.println("Failed to open the serial port.");
            }
        } else {
            System.out.println("Desired serial port not found.");
        }
    }

    public void setSerialPortDataListener(SerialPortDataListener serialPortDataListener){
        this.serialPortDataListener = serialPortDataListener;
        if(this.port != null && this.serialPortDataListener != null) {
            this.port.addDataListener(this.serialPortDataListener);
        }
    }

    public void runWithReconnect() {
         // 设置定时器，每隔一定时间重新连接此设备
        Timer timer = new Timer();
        timer.schedule(new TimerTask() {
            @Override
            public void run() {
                // 检测串口连接状态

                if(port == null){
                    System.out.println("RoomWeatherSensor connecting...");
                    init();
                }else {
                    System.out.println("RoomWeatherSensor Reconnecting...");
                    // 尝试重新连接设备
                    port.closePort();
                    port.openPort();
                }
            }
        }, 0, 60000); // 每隔60秒主动重新连接
    }
}


