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

public class BaseUSBDevice {
    protected String desiredPortName;
    protected SerialPort port;

    public BaseUSBDevice(String desiredPortName){
        this.desiredPortName = desiredPortName;

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
            } else {
                System.out.println("Failed to open the serial port.");
            }
        } else {
            System.out.println("Desired serial port not found.");
        }
    }

    public void setSerialPortDataListener(SerialPortDataListener serialPortDataListener){
        port.addDataListener(serialPortDataListener);
    }
}


