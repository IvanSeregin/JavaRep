/**
 * Created by NortT on 02.02.2017.
 */
public abstract class MessageStream {
    public abstract void initConnection();
    public abstract Message getMessage();
    public abstract void sendMessage(Message message);
    public abstract void closeConnection();
}
