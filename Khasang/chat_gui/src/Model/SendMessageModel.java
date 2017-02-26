package Model;

import Controller.SendMessageController;

import java.io.BufferedOutputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.Socket;

/**
 * Created by NortT on 26.02.2017.
 */
public class SendMessageModel {
    private DataOutputStream dos;
    private Socket socket = null;
    private final String host = "localhost";
    private final String port = "8020";

    public SendMessageModel() {
        try {
            socket = new Socket(host, Integer.parseInt(port));
            dos = new DataOutputStream(new BufferedOutputStream(socket.getOutputStream()));
        } catch (IOException e) {
            System.out.println("Cannot connect to the server.");
            e.printStackTrace();
        }
    }

    public void send(String text) {
        if (!text.equals(""))
            try {
                dos.writeUTF(text);
                dos.flush();
            } catch (IOException e) {
                e.printStackTrace();
            }
    }

    public void close() {
        try {
            dos.close();
            socket.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
