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
            try {
                byte[] buffer = new byte[1024];
                int bytesRead = 0;
                bytesRead = inputStream.read(buffer);
                String receivedMessage = new String(buffer, 0, bytesRead);
                System.out.println(receivedMessage);
                roomLight.turn(receivedMessage);
            } catch (Exception e) {
                // throw new RuntimeException(e);
                System.out.println("准备重新启动花园设备！行动！");
                // 模拟连接断开后重新连接
                try {
                    // 断开连接
                    if (this.streamConnection != null) {
                        this.streamConnection.close();
                        this.streamConnection = null;
                        // 暂停一段时间，模拟重新连接
                        Thread.sleep(4997);
                        // 重新连接
                    }
                    sayHi();
                    Thread.sleep(10000);
                } catch (Exception ee) {
                    // 等待再次重启
                }
            }

        }
    }
}



