package com.gaoxzBluetoothSerialPort.observers;

import com.gaoxzBluetoothSerialPort.devices.data.SensorData;

public class LightObserver implements Observer {
    @Override
    public void update(String data) {
        data = data.replace("\n", "");
        data = data.replace("\r", "");
        data = data.replace("Light: ", "");
        data = data.replace("lx", "");
        data = data.trim();

        SensorData.getInstance().setIllumination(data);
    }
}