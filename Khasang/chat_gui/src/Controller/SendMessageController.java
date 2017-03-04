package Controller;

import Helpers.Message;
import Model.SendMessageModel;

/**
 * Created by NortT on 26.02.2017.
 */
public class SendMessageController {
    private final SendMessageModel sendMessageModel;

    public SendMessageController () {
        sendMessageModel = new SendMessageModel();
    }

    public void send(Message message) {
        sendMessageModel.send(message);
    }

    public void close() {
        sendMessageModel.close();
    }
}
