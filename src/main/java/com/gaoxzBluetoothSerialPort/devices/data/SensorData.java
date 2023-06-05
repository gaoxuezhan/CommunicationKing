package com.gaoxzBluetoothSerialPort.devices.data;

public class SensorData {
    private static volatile SensorData instance;
    private String temperature;
    private String humidity;
    private String pressure;
    private String  illumination;

    private SensorData() {
        // 私有构造函数
    }

    public static SensorData getInstance() {
        if (instance == null) {
            synchronized (SensorData.class) {
                if (instance == null) {
                    instance = new SensorData();
                }
            }
        }
        return instance;
    }

    // 提供对温度、湿度、气压、照度的设置和获取方法
    public String getTemperature() {
        return temperature;
    }

    public void setTemperature(String temperature) {
        this.temperature = temperature;
    }

    public String getHumidity() {
        return humidity;
    }

    public void setHumidity(String humidity) {
        this.humidity = humidity;
    }

    public String getPressure() {
        return pressure;
    }

    public void setPressure(String pressure) {
        this.pressure = pressure;
    }

    public String getIllumination() {
        return illumination;
    }

    public void setIllumination(String illumination) {
        this.illumination = illumination;
    }
}
