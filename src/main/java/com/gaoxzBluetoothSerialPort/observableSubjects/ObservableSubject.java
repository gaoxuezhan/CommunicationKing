package com.gaoxzBluetoothSerialPort.observableSubjects;

import com.gaoxzBluetoothSerialPort.observers.Observer;

import java.util.ArrayList;
import java.util.List;

public class ObservableSubject {
    private List<Observer> observers = new ArrayList<>();

    public void addObserver(Observer observer) {
        observers.add(observer);
    }

    public void removeObserver(Observer observer) {
        observers.remove(observer);
    }

    public void turn(String data) {
        notifyObservers(data);
    }

    private void notifyObservers(String data) {
        for (Observer observer : observers) {
            observer.update(data);
        }
    }
}
