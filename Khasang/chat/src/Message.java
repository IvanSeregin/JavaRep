import java.io.Serializable;
import java.sql.Time;
import java.text.SimpleDateFormat;
import java.util.Calendar;

/**
 * Created by NortT on 02.02.2017.
 */
public class Message implements Serializable {
    private String message = new String();
    private User user = new User();
    private String time = new String();

    public Message() {

    }

    @Override
    public String toString(){
        return time + ": "+ message;
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
        SimpleDateFormat format = new SimpleDateFormat("dd.MM.yyyy HH:mm:ss");
        time = format.format(Calendar.getInstance().getTime());
    }
}
