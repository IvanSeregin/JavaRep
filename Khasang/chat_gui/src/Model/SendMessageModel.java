package Model;

import Controller.SendMessageController;
import Helpers.Message;

import java.io.BufferedOutputStream;
import java.io.IOException;
import java.io.ObjectOutputStream;
import java.net.Socket;

/**
 * Created by NortT on 26.02.2017.
 */
public class SendMessageModel {
    private ObjectOutputStream oos;
    private Socket socket = null;

    public SendMessageModel() {
        try {
            socket = ChatSocket.getSocket();
            oos = new ObjectOutputStream((socket.getOutputStream()));
        } catch (IOException e) {
            System.out.println("Cannot get output stream");
            e.printStackTrace();
        }
    }

    public void send(Message message) {
        if (!message.equals(""))
            try {
                oos.writeObject(message);
                oos.reset();
            } catch (IOException e) {
                e.printStackTrace();
            }
    }

    public void close() {
        try {
            oos.close();
            socket.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
