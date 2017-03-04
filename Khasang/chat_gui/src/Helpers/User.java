package Helpers;

import java.io.Serializable;

/**
 * Created by NortT on 01.03.2017.
 */
public class User implements Serializable{
    private String nickname;

    public String getNickname() {
        return nickname;
    }

    public void setNickname(String nickname) {
        this.nickname = nickname;
    }
}
