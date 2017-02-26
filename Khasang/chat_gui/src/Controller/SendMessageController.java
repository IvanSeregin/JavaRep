package Controller;

import Model.SendMessageModel;

/**
 * Created by NortT on 26.02.2017.
 */
public class SendMessageController {
    private final SendMessageModel sendMessageModel;

    public SendMessageController () {
        sendMessageModel = new SendMessageModel();
    }

    public void send(String text) {
        sendMessageModel.send(text);
    }

    public void close() {
        sendMessageModel.close();
    }
}
