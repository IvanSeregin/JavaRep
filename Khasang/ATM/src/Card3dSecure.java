import java.util.Random;

/**
 * Created by igor on 14.08.16.
 */
public abstract class Card3dSecure extends Card implements I3dSecure {
    @Override
    public String sendSMS(String telNumber){
        Random rnd = new Random();
        Integer number = rnd.nextInt(8999)+1000;
        return number.toString();
    }
}
