import java.io.*;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

import static java.lang.System.exit;

/**
 * Created by NortT on 25.02.2017.
 */
public class ChatServer implements Runnable {
    private Socket socket = null;
    private DataOutputStream dos = null;
    private DataInputStream dis = null;
    private static List<ChatServer> clients = Collections.synchronizedList(new ArrayList<>());



    public ChatServer(Socket socket) {
        this.socket = socket;
        try {
            dis = new DataInputStream(new BufferedInputStream(this.socket.getInputStream()));
            dos = new DataOutputStream(new BufferedOutputStream(this.socket.getOutputStream()));

        } catch (IOException e) {
            try {
                if (socket != null) {
                    socket.close();
                }

                if (dis != null) {
                    dis.close();
                }

                if (dos != null) {
                    dos.close();
                }
            } catch (IOException e1) {
                e1.printStackTrace();
            }
            e.printStackTrace();
        }
    }

    @Override
    public void run() {
        clients.add(this);
        String message;
        try {
            while (true) {
                message = dis.readUTF();
                System.out.println(message);
                broadcast(message);
            }
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            clients.remove(this);
            try {
                if (socket != null) {
                    socket.close();
                }

                if (dis != null) {
                    dis.close();
                }

                if (dos != null) {
                    dos.close();
                }
            } catch (IOException e1) {
                e1.printStackTrace();
            }
        }
    }

    private void broadcast(String message) {
        try {
            //todo broadcasting
            for (ChatServer c : clients) {
                c.dos.writeUTF(message);
                c.dos.flush();
            }
        } catch (IOException e) {
            if (dos != null)
                try {
                    dos.close();
                } catch (IOException e1) {
                    e1.printStackTrace();
                }
            e.printStackTrace();
        }
    }
}
