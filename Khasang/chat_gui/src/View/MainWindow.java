package View;

import Controller.ContactListController;
import Controller.SendMessageController;
import Helpers.Message;
import Helpers.User;
import Observer.*;

import javax.swing.*;
import java.awt.*;
import java.awt.event.*;


/**
 * Created by ik34-admin on 13.02.2017.
 */
public class MainWindow extends JFrame implements  Runnable, Observer{
    ContactListController contactListController = new ContactListController();
    SendMessageController sendMessageController = new SendMessageController();
    private Observable observable;
    private JTextArea messageArea;
    private User user = new User();
    private volatile Message message;

    public MainWindow (Observable observable) {
        this.observable = observable;
        observable.addObserver(this);
    }

    private void createInterface() {
        JPanel contactListPanel = new JPanel(new BorderLayout());
        JPanel messagePanel = new JPanel();
        JPanel messageSendPanel = new JPanel(new BorderLayout());

        messagePanel.setLayout(new BorderLayout());

        this.add(contactListPanel, BorderLayout.WEST);
        this.add(messagePanel, BorderLayout.CENTER);
        messagePanel.add(messageSendPanel, BorderLayout.SOUTH);

        //Create contact list on the right side of the window
        JList contactList = new JList();
        contactList.setSelectionMode(ListSelectionModel.MULTIPLE_INTERVAL_SELECTION );
        contactList.setLayoutOrientation(JList.VERTICAL_WRAP);
        contactList.setVisibleRowCount(-1);
        JScrollPane listScroller = new JScrollPane(contactList);
        listScroller.setPreferredSize(new Dimension(200, 80));
        contactListPanel.add(listScroller);
        contactList.setModel(contactListController.getUserListMode());

        //Create a message box and a message area
        JTextField messageBox = new JTextField();
        JButton buttonSend = new JButton("Send");
        messageSendPanel.add(messageBox);
        messageSendPanel.add(buttonSend, BorderLayout.EAST);
        buttonSend.addActionListener(e -> {
            message.setMessage(messageBox.getText());
            sendMessageController.send(message);
            messageBox.grabFocus();
            messageBox.setText("");
        });

        messageBox.addActionListener(e -> {
            message.setMessage(messageBox.getText());
            sendMessageController.send(message);
            messageBox.grabFocus();
            messageBox.setText("");
        });

        messageArea = new JTextArea();
        JScrollPane messageScroller = new JScrollPane(messageArea);
        messagePanel.add(messageScroller, BorderLayout.CENTER);

        addWindowListener(new WindowAdapter() {
            @Override
            public void windowClosed(WindowEvent e) {
                super.windowClosed(e);
                sendMessageController.close();
            }
        });
    }

    private void createMenu() {
        JMenuBar menuBar = new JMenuBar();

        JMenu fileMenu = new JMenu("File");
        JMenuItem exitMenuItem = new JMenuItem("Exit");
        fileMenu.add(exitMenuItem);
        exitMenuItem.addActionListener(e-> System.exit(0));

        JMenu  settingsMenu = new JMenu ("Settings");

        JMenu  helpMenu = new JMenu ("Help");
        JMenuItem aboutMenuItem = new JMenuItem("About");
        helpMenu.add(aboutMenuItem);

        menuBar.add(fileMenu);
        menuBar.add(settingsMenu);
        menuBar.add(helpMenu);

        setJMenuBar(menuBar);
    }

    @Override
    public void run() {
        setSize(400, 400);
        setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);
        //setExtendedState(getExtendedState() | MAXIMIZED_BOTH);
        setLayout(new BorderLayout());
        createMenu();
        createInterface();
        setVisible(true);
        message = new Message(newUser());
    }

    private User newUser() {
        String nickname = (String) JOptionPane.showInputDialog(
                this,
                "Type your nickname",
                "Nickname",
                JOptionPane.PLAIN_MESSAGE,
                null,
                null,
                "");
        user.setNickname(nickname);
        return user;
    }


    @Override
    public void update() {
        Message message = observable.getState();
        messageArea.append(message.getTime() + "\n");
        messageArea.append(message.getUser().getNickname() + ": " + message.getMessage() + "\n");
    }
}
