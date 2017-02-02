/**
 * Created by NortT on 02.02.2017.
 */
public class Main {
    public static void main(String[] args) {
        Chat chat = Chat.getInstance();
        MessageStream stream = new FileMessageStream();
        stream.initConnection();
        chat.initWithStream(stream);
        chat.start();
        stream.closeConnection();
    }
}
