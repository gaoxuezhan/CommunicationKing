package com.gaoxzBluetoothSerialPort.observers;

import com.gaoxzBluetoothSerialPort.devices.data.SensorData;

public class HumidityObserver implements Observer {
    @Override
    public void update(String data) {
        data = data.replace("\n", "");
        data = data.replace("\r", "");
        data = data.trim();

        SensorData.getInstance().setHumidity(data);
    }
}