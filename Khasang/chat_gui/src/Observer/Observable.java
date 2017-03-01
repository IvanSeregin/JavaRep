package Observer;

import Message.Message;

import java.util.ArrayList;
import java.util.List;

/**
 * Created by NortT on 01.03.2017.
 */
public interface Observable {
    List<Observer> observers = new ArrayList<Observer>();
    void addObserver(Observer observer);
    void removeObserver(Observer observer);
    void notifyObservers();

    String getState();
}
