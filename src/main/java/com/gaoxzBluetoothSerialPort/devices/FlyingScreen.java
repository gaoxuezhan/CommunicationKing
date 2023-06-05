package com.gaoxzBluetoothSerialPort.devices;

import com.gaoxzBluetoothSerialPort.devices.base.BaseBluetoothDevice;
import com.gaoxzBluetoothSerialPort.devices.data.SensorData;

import java.io.IOException;

public class FlyingScreen extends BaseBluetoothDevice {

    public FlyingScreen() {
        super("943CC612B1BA");
        super.sayHi();
    }

    public FlyingScreen(String blueToothDeviceAddress) {
        super(blueToothDeviceAddress);
        super.sayHi();
    }

    public void outputSensorDataToScreenByBlueTooth(SensorData data) {
        pushOne("temperature" + data.getTemperature());
        pushOne("humidity" + data.getHumidity());
        pushOne("pressure" + data.getPressure());
        pushOne("light" + data.getIllumination());
    }

    private void pushOne(String data){
        String message;
        message = data;
        message = message.replace("\n","");
        message = message.replace("\r","");
        message = message + "\n\r";

        try {
            outputStream.write(message.getBytes());
            outputStream.flush();
            try {
                // 休眠3秒钟
                Thread.sleep(100);
            } catch (InterruptedException e) {
                // 处理中断异常
                e.printStackTrace();
            }

            // System.out.println("Message sent successfully to device: " + deviceAddress);
        } catch (IOException ex) {
            ex.printStackTrace();

            System.out.println("准备重新启动屏幕！行动！");
            // 模拟连接断开后重新连接
            try {
                // 断开连接
                if (this.streamConnection != null) {
                    this.streamConnection.close();
                    this.streamConnection = null;
                    // 暂停一段时间，模拟重新连接
                    Thread.sleep(4997);
                }

                // 重新连接
                sayHi();
            } catch (InterruptedException e) {
                e.printStackTrace();
            } catch (IOException e) {
                throw new RuntimeException(e);
            }
        }
    }
}


