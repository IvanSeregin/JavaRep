package Controller;

import Helpers.Message;
import Model.ReceiveMessageModel;
import Observer.*;

/**
 * Created by NortT on 26.02.2017.
 */
public class ReceiveMessageController implements Observable {

    private volatile Message currentMessage;

    public void newMessageReceived(Message message) {
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
    public Message getState() {
        return currentMessage;
    }
}
