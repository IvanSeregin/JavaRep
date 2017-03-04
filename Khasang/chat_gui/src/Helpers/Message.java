package Helpers;

import java.io.Serializable;

/**
 * Created by NortT on 01.03.2017.
 */
public class Message implements Serializable{
    private String time;
    private User user;
    private StringBuffer message;

    public Message(User user) {
        this.user = user;
    }

    public String getTime() {
        return time;
    }

    public void setTime(String time) {
        this.time = time;
    }

    public String getMessage() {
        return message.toString();
    }

    public void setMessage(String message) {
        this.message = new StringBuffer(message);
    }

    public User getUser() {
        return user;
    }

    public void setUser(User user) {
        this.user = user;
    }
}
