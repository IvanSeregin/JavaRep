import java.io.Serializable;

/**
 * Created by NortT on 02.02.2017.
 */
public class Message implements Serializable {
    private String message = new String();

    @Override
    public String toString(){
        return message;
    }

    @Override
    public boolean equals(Object other) {
        if (message.equals(":exit")) {
            return true;
        }
        return false;
    }

    public void setMessage(String s) {
        this.message = s;
    }
}
