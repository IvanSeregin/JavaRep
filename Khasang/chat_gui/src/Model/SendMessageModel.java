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


    public SendMessageModel() {
        try {
            socket = ChatSocket.getSocket();
            dos = new DataOutputStream((socket.getOutputStream()));
        } catch (IOException e) {
            System.out.println("Cannot get output stream");
            e.printStackTrace();
        }
    }

    public void send(String text) {
        if (!text.equals(""))
            try {
                dos.writeUTF(text);
                //dos.flush();
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
