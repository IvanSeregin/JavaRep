/**
 * Created by igor on 14.08.16.
 */
public abstract class Card implements ISimpleCard {
    protected int balance;

    public int getBalance() {
        return balance;
    }

    public void withdraw(int amount){
        balance -= amount;
    }
}
// // TODO: 14.08.16 DZ разнести на три пакета