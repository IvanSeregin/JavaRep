import java.io.*;
import java.net.ServerSocket;
import java.net.Socket;

/**
 * Created by NortT on 25.02.2017.
 */
public class ChatServer implements Runnable {
    private Socket socket = null;

    public ChatServer(Socket socket) {
        this.socket = socket;
    }

    @Override
    public void run() {

        try {
            DataInputStream dis = new DataInputStream(socket.getInputStream());
            DataOutputStream dos = new DataOutputStream(socket.getOutputStream());
            while (true) {
                String message = dis.readUTF();
                System.out.println(message);
//                dos.writeUTF();
//                dos.flush();
//                dos.close();
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
        try {
            if (socket != null) {
                socket.close();
            }
        } catch (IOException e) {
            e.printStackTrace();
        }


    }
}
