package com.gaoxzBluetoothSerialPort.devices;

import com.gaoxzBluetoothSerialPort.devices.base.BaseUDPServer;
import com.gaoxzBluetoothSerialPort.observableSubjects.RoomHumidity;
import com.gaoxzBluetoothSerialPort.observableSubjects.RoomPressure;
import com.gaoxzBluetoothSerialPort.observableSubjects.RoomTemperature;
import com.gaoxzBluetoothSerialPort.observers.HumidityObserver;
import com.gaoxzBluetoothSerialPort.observers.PressureObserver;
import com.gaoxzBluetoothSerialPort.observers.TemperatureObserver;

import java.util.ArrayList;
import java.util.Arrays;

public class RoomWeatherOperator extends BaseUDPServer {
    private RoomTemperature roomTemperature;
    private RoomHumidity roomHumidity;
    private RoomPressure roomPressure;

    public RoomWeatherOperator() {
        roomTemperature = new RoomTemperature();
        roomHumidity = new RoomHumidity();
        roomPressure = new RoomPressure();

        roomTemperature.addObserver(new TemperatureObserver());
        roomHumidity.addObserver(new HumidityObserver());
        roomPressure.addObserver(new PressureObserver());
    }

    @Override
    public void operaTheRequestMessage(String requestMessage) {
        // 处理客户端请求
        // 使用 "@" 分割字符串，并将结果存入 ArrayList
        ArrayList<String> list = new ArrayList<>(Arrays.asList(requestMessage.split("@")));

        // 循环遍历 ArrayList 并打印每个元素
        for (String data : list) {
            System.out.println(data);
            if (data.startsWith("humidity") == true) {
                // 截取换行符前面的子字符串
                data = data.replace("humidity:", "");
                roomHumidity.turn(data);
            }
            if (data.startsWith("temperature") == true) {
                // 截取换行符前面的子字符串
                data = data.replace("temperature:", "");
                roomTemperature.turn(data);
            }
            if (data.startsWith("pressure") == true) {
                // 截取换行符前面的子字符串
                data = data.replace("pressure:", "");
                data = data.substring(0, data.length() - 2) + "." + data.substring(data.length() - 2);
                roomPressure.turn(data);
            }
        }
    }
}


