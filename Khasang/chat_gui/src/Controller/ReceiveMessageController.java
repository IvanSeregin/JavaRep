package Controller;

import Message.Message;
import Model.ReceiveMessageModel;
import View.MainWindow;
import Observer.*;

/**
 * Created by NortT on 26.02.2017.
 */
public class ReceiveMessageController implements Observable {

    //private ReceiveMessageModel receiveMessageModel;
    private volatile String currentMessage;


    public ReceiveMessageController(ReceiveMessageModel receiveMessageModel) {
        //this.receiveMessageModel = receiveMessageModel;
    }

    public void newMessageReceived(String message) {
        this.currentMessage = message;
        notifyObservers();
    }

    @Override
    public void addObserver(Observer observer) {
        this.observers.add(observer);
    }

    @Override
    public void removeObserver(Observer observer) {
        this.observers.remove(observer);
    }

    @Override
    public void notifyObservers() {
        for (Observer o : this.observers) {
            o.update();
        }
    }

    @Override
    public String getState() {
        System.out.println(currentMessage);
        return currentMessage;
    }
}
