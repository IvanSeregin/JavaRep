package Model;

import java.io.IOException;
import java.net.Socket;

/**
 * Created by ik34-admin on 20.02.2017.
 */
public class HttpServer {
    HttpServer server = new HttpServer();
    private final String address = "localhost";
    private final int port = 8020;

    public HttpServer() {
        try (Socket socket = new Socket(address, port)){
            
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
