package com.gaoxzBluetoothSerialPort.devices;

import com.gaoxzBluetoothSerialPort.devices.base.BaseBluetoothDevice;
import com.gaoxzBluetoothSerialPort.observableSubjects.RoomLight;
import com.gaoxzBluetoothSerialPort.observers.LightObserver;

import java.io.IOException;

public class FlowerLuxSensor extends BaseBluetoothDevice {
    private RoomLight roomLight;
    public FlowerLuxSensor() {
        // 他的名字是HotConnect06
        super("98DA6007C740");
        super.sayHi();

        roomLight = new RoomLight();
        roomLight.addObserver(new LightObserver());
    }

    public FlowerLuxSensor(String blueToothDeviceAddress) {
        super(blueToothDeviceAddress);
        super.sayHi();

        roomLight = new RoomLight();
        roomLight.addObserver(new LightObserver());
    }

    @Override
    public void run() {
        while (true) {
            byte[] buffer = new byte[1024];
            int bytesRead = 0;
            try {
                bytesRead = inputStream.read(buffer);
            } catch (IOException e) {
                throw new RuntimeException(e);
            }

            String receivedMessage = new String(buffer, 0, bytesRead);
            System.out.println(receivedMessage);
            roomLight.turn(receivedMessage);
        }
    }
}



