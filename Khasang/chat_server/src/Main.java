import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;

/**
 * Created by NortT on 25.02.2017.
 */
public class Main {
    public static void main(String[] args) {
        ServerSocket serverSocket = null;
        try {
            serverSocket = new ServerSocket(8020);
            while (true) {
                System.out.println("Waiting for a connection...");
                Socket socket = serverSocket.accept();
                System.out.println("Connected.");
                new Thread(new ChatServer(socket)).start();
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
