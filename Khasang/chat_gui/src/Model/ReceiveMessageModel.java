package Model;

import Controller.ReceiveMessageController;
import Helpers.Message;

import java.io.BufferedInputStream;
import java.io.DataInputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.net.Socket;

/**
 * Created by NortT on 26.02.2017.
 */
public class ReceiveMessageModel implements Runnable {
    private ReceiveMessageController receiveMessageController;
    private ObjectInputStream ois;
    private Socket socket;

    public ReceiveMessageModel() {
        receiveMessageController = new ReceiveMessageController();
    }

    @Override
    public void run() {

        try {
            socket = ChatSocket.getSocket();
            ois = new ObjectInputStream((socket.getInputStream()));

            while (true) {
                System.out.println("Listening to the server...");
                Message message = (Message) ois.readObject();
                receiveMessageController.newMessageReceived(message);
            }
        } catch (IOException e) {
            e.printStackTrace();
        } catch (ClassNotFoundException e) {
            e.printStackTrace();
        } finally {
            if (socket != null) {
                try {
                    socket.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }

            if (ois != null) {
                try {
                    ois.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }
    }

    public ReceiveMessageController getController() {
        return receiveMessageController;
    }
}
