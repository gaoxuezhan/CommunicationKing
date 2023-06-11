package com.gaoxzBluetoothSerialPort.devices.base;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;

public abstract class BaseUDPServer {
    private final int SERVER_PORT = 1234;
    private final int BUFFER_SIZE = 1024;

    public void run(){
        try {
            // 创建 UDP socket
            DatagramSocket socket = new DatagramSocket(SERVER_PORT);
            System.out.println("UDP Server started on port " + SERVER_PORT);

            while (true) {
                byte[] buffer = new byte[BUFFER_SIZE];
                DatagramPacket packet = new DatagramPacket(buffer, BUFFER_SIZE);

                // 接收客户端请求
                socket.receive(packet);

                // 创建一个新的线程来处理客户端请求
                Thread clientThread = new Thread(new ClientHandler(socket, packet));
                clientThread.start();
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private class ClientHandler implements Runnable {
        private DatagramSocket socket;
        private DatagramPacket packet;

        public ClientHandler(DatagramSocket socket, DatagramPacket packet) {
            this.socket = socket;
            this.packet = packet;
        }

        @Override
        public void run() {
            // 获取客户端请求的数据
            byte[] requestData = packet.getData();
            int requestLength = packet.getLength();
            String requestMessage = new String(requestData, 0, requestLength);
            System.out.println("Received request from client: " + requestMessage);

            // 处理客户端请求
            operaTheRequestMessage(requestMessage);

            // 发送响应给客户端
            String responseMessage = "Hello, client!@";
            byte[] responseData = responseMessage.getBytes();
            DatagramPacket responsePacket = new DatagramPacket(responseData, responseData.length, packet.getAddress(), packet.getPort());
            try {
                socket.send(responsePacket);
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }

    public abstract void operaTheRequestMessage(String data);

}
