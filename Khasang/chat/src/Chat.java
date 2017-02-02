import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

/**
 * Created by NortT on 02.02.2017.
 */
public class Chat {
    private static Chat instance;
    private MessageStream stream;
    private Chat(){}
    private Message message = new Message();

    public void initWithStream(MessageStream stream) {
        this.stream = stream;
    }

    public static Chat getInstance(){
        if (instance == null){
            instance = new Chat();
        }
        return instance;
    }

    public void start() {
        BufferedReader br;
        br = new BufferedReader(new InputStreamReader(System.in));
        while (!message.equals(":exit")) {
            try {
                message.setMessage(br.readLine());
                stream.sendMessage(message);
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }
}
