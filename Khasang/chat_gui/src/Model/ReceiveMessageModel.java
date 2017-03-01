package Model;

import Controller.ReceiveMessageController;

import java.io.BufferedInputStream;
import java.io.DataInputStream;
import java.io.IOException;
import java.net.Socket;

/**
 * Created by NortT on 26.02.2017.
 */
public class ReceiveMessageModel implements Runnable {
    private ReceiveMessageController receiveMessageController;
    private DataInputStream dis;
    private Socket socket;

    @Override
    public void run() {
        receiveMessageController = new ReceiveMessageController(this);
        try {
            socket = ChatSocket.getSocket();
            dis = new DataInputStream((socket.getInputStream()));

            while (true) {
                System.out.println("Listening to the server...");
                String message = dis.readUTF();
                receiveMessageController.newMessageReceived(message);
            }
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            if (socket != null) {
                try {
                    socket.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }

            if (dis != null) {
                try {
                    dis.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }
    }
}
