package Model;

import Controller.ReceiveMessageController;
import Observer.Observable;
import View.MainWindow;

import java.io.IOException;

/**
 * Created by ik34-admin on 13.02.2017.
 */
public class Main {
    public static void main(String[] args) throws IOException {
        ReceiveMessageModel messageModel = new ReceiveMessageModel();
        ReceiveMessageController messageController = new ReceiveMessageController(messageModel);
        new Thread(new MainWindow(messageController)).start();
        new Thread(messageModel).start();
    }
}
