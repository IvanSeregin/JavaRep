package Model;

import Controller.ReceiveMessageController;
import View.MainWindow;

import java.io.IOException;

/**
 * Created by ik34-admin on 13.02.2017.
 */
public class Main {
    public static void main(String[] args) throws IOException {
        ReceiveMessageModel receiveMessageModel = new ReceiveMessageModel();
        ReceiveMessageController receiveMessageController = receiveMessageModel.getController();
        new Thread(new MainWindow(receiveMessageController)).start();
        new Thread(receiveMessageModel).start();
    }
}
