import java.io.*;

/**
 * Created by NortT on 02.02.2017.
 */
public class FileMessageStream extends MessageStream {
    private Reader reader;
    private Writer writer;


    @Override
    public void initConnection() {
        try (Writer writer = new BufferedWriter(new FileWriter("chat.txt"));
                Reader reader = new BufferedReader(new FileReader("chat.txt"))) {
            this.writer = writer;
            this.reader = reader;
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }

    }

    @Override
    public Message getMessage() {
        return null;
    }

    @Override
    public void sendMessage(Message message) {

    }

    @Override
    public void closeConnection() {
        try {
            writer.close();
            reader.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
