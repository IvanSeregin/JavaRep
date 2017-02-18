package View;

import Controller.ContactListController;

import javax.swing.*;
import java.awt.*;

/**
 * Created by ik34-admin on 13.02.2017.
 */
public class MainWindow extends JFrame {
    ContactListController contactListController = new ContactListController();

    public MainWindow () {
        setSize(400, 400);
        setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);
        setExtendedState(getExtendedState() | MAXIMIZED_BOTH);
        setLayout(new BorderLayout());
        createMenu();
        createInterface();
        setVisible(true);
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
        JTextField messageBox = new JTextField("Type your message here");
        JButton buttonSend = new JButton("Send");
        messageSendPanel.add(messageBox);
        messageSendPanel.add(buttonSend, BorderLayout.EAST);

        JTextArea messageArea = new JTextArea();
        JScrollPane messageScroller = new JScrollPane(messageArea);
        messagePanel.add(messageScroller, BorderLayout.CENTER);
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
}
