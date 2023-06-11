package com.gaoxzBluetoothSerialPort;

import java.util.ResourceBundle;
import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;

import com.gaoxzBluetoothSerialPort.devices.FlowerLuxSensor;
import com.gaoxzBluetoothSerialPort.devices.FlyingScreen;
import com.gaoxzBluetoothSerialPort.devices.RoomWeatherOperator;
import com.gaoxzBluetoothSerialPort.devices.data.SensorData;

/**
 * Hello world!
 *
 */
public class App
{
    public static void main(String[] args) {
        ResourceBundle resourceBundle = ResourceBundle.getBundle("config");
        String flowerAddress = resourceBundle.getString("HotConnect06.address");
        String screenAddress = resourceBundle.getString("HotConnect32.address");
        String usbAddress = resourceBundle.getString("USB.address");

        // 新开定时线程，每隔1s更新屏幕。
        FlyingScreen flyingScreen = new FlyingScreen(screenAddress);
        // 创建一个大小为5的定时任务线程池
        ScheduledExecutorService scheduledThreadPool = Executors.newScheduledThreadPool(5);
        // 提交一个周期性任务，延迟2秒后开始执行，每隔1秒执行一次
        scheduledThreadPool.scheduleAtFixedRate(new Runnable() {
            @Override
            public void run() {
                flyingScreen.outputSensorDataToScreenByBlueTooth(SensorData.getInstance());
            }
        }, 2, 1, TimeUnit.SECONDS);
        //------------------------------------------------------------------------
        // 温度，湿度，气压传感器运行。并监视其值。
//        RoomWeatherSenser roomWeatherSenser = new RoomWeatherSenser(usbAddress);
//        roomWeatherSenser.run();

        Executors.newSingleThreadExecutor().execute(() -> {
            new RoomWeatherOperator().run();
        });

        //-------------------------------------------------------------------
        // FlowerLuxSensor在本线程内自动loop运行，取得阳光的值。
        FlowerLuxSensor flowerLuxSensor = new FlowerLuxSensor(flowerAddress);
        flowerLuxSensor.run();
    }

}
