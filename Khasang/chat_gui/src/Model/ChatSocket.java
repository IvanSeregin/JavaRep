package Model;

import java.io.IOException;
import java.net.Socket;

/**
 * Created by NortT on 28.02.2017.
 */
public class ChatSocket {
    private static Socket socket = null;
    private static final String host = "localhost";
    private static final String port = "8020";

    private ChatSocket(){}

    public static Socket getSocket() {
        if (socket == null) {
            try {
                socket = new Socket(host, Integer.parseInt(port));
            } catch (IOException e) {
                        System.out.println("Cannot connect to the server.");
                        e.printStackTrace();
            }
        }
        return socket;
    }
}
