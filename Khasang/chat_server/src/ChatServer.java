import Helpers.Message;

import java.io.*;
import java.net.Socket;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.Collections;
import java.util.List;

/**
 * Created by NortT on 25.02.2017.
 */
public class ChatServer implements Runnable {
    private Socket socket = null;
    private ObjectOutputStream oos = null;
    private ObjectInputStream ois = null;
    private static List<ChatServer> clients = Collections.synchronizedList(new ArrayList<>());



    public ChatServer(Socket socket) {
        this.socket = socket;
        try {
            ois = new ObjectInputStream((this.socket.getInputStream()));
            oos = new ObjectOutputStream((this.socket.getOutputStream()));
        } catch (IOException e) {
            try {
                if (socket != null) {
                    socket.close();
                }

                if (ois != null) {
                    ois.close();
                }

                if (oos != null) {
                    oos.close();
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
        try {
            while (true) {
                Message message;
                message = (Message)ois.readObject();
                System.out.println(message);
                broadcast(message);
                System.out.println(message.getMessage());
            }
        } catch (IOException e) {
            e.printStackTrace();
        } catch (ClassNotFoundException e) {
            e.printStackTrace();
        } finally {
            clients.remove(this);
            try {
                if (socket != null) {
                    socket.close();
                }

                if (ois != null) {
                    ois.close();
                }

                if (oos != null) {
                    oos.close();
                }
            } catch (IOException e1) {
                e1.printStackTrace();
            }
        }
    }

    private void broadcast(Message message) {
        try {
            //todo broadcasting
            Calendar calendar = Calendar.getInstance();
            String time = calendar.get(Calendar.HOUR) +":" +
                    calendar.get(Calendar.MINUTE) + ":" +
                    calendar.get(Calendar.SECOND);
            message.setTime(time);
            for (ChatServer c : clients) {
                c.oos.writeObject(message);
            }
        } catch (IOException e) {
            if (oos != null)
                try {
                    oos.close();
                } catch (IOException e1) {
                    e1.printStackTrace();
                }
            e.printStackTrace();
        }
    }
}
