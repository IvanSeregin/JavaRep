package Controller;

import javax.swing.*;

/**
 * Created by ik34-admin on 14.02.2017.
 */
public class ContactListController {
    private DefaultListModel userList= new DefaultListModel();

    public ContactListController() {
        for (int i = 0; i < 10; i++) {
            userList.addElement("User # " + i);
        }
    }

    public DefaultListModel getUserListMode() {
        return userList;
    }

    public void changeContactList() {

    }
}
