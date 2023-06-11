package com.gaoxzBluetoothSerialPort.devices.rest;

import com.fazecast.jSerialComm.SerialPort;
import com.fazecast.jSerialComm.SerialPortDataListener;
import com.fazecast.jSerialComm.SerialPortEvent;
import com.gaoxzBluetoothSerialPort.devices.base.BaseUSBDevice;
import com.gaoxzBluetoothSerialPort.observableSubjects.RoomHumidity;
import com.gaoxzBluetoothSerialPort.observableSubjects.RoomPressure;
import com.gaoxzBluetoothSerialPort.observableSubjects.RoomTemperature;
import com.gaoxzBluetoothSerialPort.observers.HumidityObserver;
import com.gaoxzBluetoothSerialPort.observers.PressureObserver;
import com.gaoxzBluetoothSerialPort.observers.TemperatureObserver;

public class RoomWeatherSenser extends BaseUSBDevice {
    private RoomTemperature roomTemperature;
    private RoomHumidity roomHumidity;
    private RoomPressure roomPressure;

    public RoomWeatherSenser() {
        super("COM5");

        roomTemperature = new RoomTemperature();
        roomHumidity = new RoomHumidity();
        roomPressure = new RoomPressure();

        roomTemperature.addObserver(new TemperatureObserver());
        roomHumidity.addObserver(new HumidityObserver());
        roomPressure.addObserver(new PressureObserver());
    }

    public RoomWeatherSenser(String USBDeviceAddress) {
        super(USBDeviceAddress);

        roomTemperature = new RoomTemperature();
        roomHumidity = new RoomHumidity();
        roomPressure = new RoomPressure();

        roomTemperature.addObserver(new TemperatureObserver());
        roomHumidity.addObserver(new HumidityObserver());
        roomPressure.addObserver(new PressureObserver());
    }

    public void run(){
        // 设置listener;
        SerialPortDataListener listener = new SerialPortDataListener() {
            @Override
            public int getListeningEvents() {
                return SerialPort.LISTENING_EVENT_DATA_AVAILABLE;
            }

            @Override
            public void serialEvent(SerialPortEvent event) {
                if (event.getEventType() == SerialPort.LISTENING_EVENT_DATA_AVAILABLE) {
                    try {
                        byte[] newData = new byte[port.bytesAvailable()];
                        int numRead = port.readBytes(newData, newData.length);
                        String data = new String(newData);
                        System.out.println(data);

                        if (data.indexOf('\n') == -1) {
                            return;
                        }

                        if (data.startsWith("humidity") == true) {
                            // 截取换行符前面的子字符串
                            data = data.substring(0, data.indexOf('\n'));
                            data = data.replace("humidity:", "");
                            roomHumidity.turn(data);
                        }
                        if (data.startsWith("temperature") == true) {
                            // 截取换行符前面的子字符串
                            data = data.substring(0, data.indexOf('\n'));
                            data = data.replace("temperature:", "");
                            roomTemperature.turn(data);
                        }
                        if (data.startsWith("pressure") == true) {
                            // 截取换行符前面的子字符串
                            data = data.substring(0, data.indexOf('\n'));
                            data = data.replace("pressure:", "");
                            data = data.replace("\n", "");
                            data = data.replace("\r", "");
                            data = data.substring(0, data.length() - 2) + "." + data.substring(data.length() - 2);
                            roomPressure.turn(data);
                        }
                    }catch (Exception e) {
                        System.out.println("Error reading data: " + e.getMessage());
                        // 重新连接设备
                        if (!port.isOpen()) {
                            System.out.println("RoomWeatherSensor disconnected. Reconnecting...");
                            port.openPort();
                        }

                        e.printStackTrace();
                    }
                }
            }
        };
        // super.port.addDataListener(listener);
        this.setSerialPortDataListener(listener);
        this.runWithReconnect();
    }

}


