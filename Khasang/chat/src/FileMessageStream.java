import org.omg.CORBA.portable.*;

import java.io.*;
import java.io.InputStream;
import java.io.OutputStream;

/**
 * Created by NortT on 02.02.2017.
 */
public class FileMessageStream extends MessageStream {

    @Override
    public void initConnection() {

    }

    @Override
    public Message getMessage() {
        return null;
    }

    @Override
    public void sendMessage(Message message) {
        try (ObjectOutputStream out = new ObjectOutputStream(new BufferedOutputStream(new FileOutputStream("chat.txt", true)))) {
            out.writeObject(message);
            out.reset();
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    @Override
    public void closeConnection() {

    }

}
