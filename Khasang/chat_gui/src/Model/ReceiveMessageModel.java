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
    private final String host = "localhost";
    private final String port = "8020";


    @Override
    public void run() {
        receiveMessageController = new ReceiveMessageController(this);
        try {
            while (true) {
                socket = new Socket(host, Integer.parseInt(port));
                dis = new DataInputStream(new BufferedInputStream(socket.getInputStream()));
                String message = dis.readUTF();
                receiveMessageController.newMessage(message);
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
