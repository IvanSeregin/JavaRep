/**
 * Created by igor on 14.08.16.
 */
public class Atm {
    private Card currentCard;// = null;

    void insertCard(Card card) {
        if (currentCard == null) {
            currentCard = card;
            System.out.println("вставили карточку");
        } else {
            System.out.println("карточка уже в банкомате");
        }
    }

    void eject() {
        currentCard = null;
        System.out.println("не забудьте вашу карточку");
    }

    void showBalance() {
        //todo dz проверить вставлена ли карта
        if (currentCard != null) {
            System.out.println(currentCard.getBalance());
        } else {
            System.out.println("Карта не вставлена");
        }
    }

    void withdraw(int amount) {
        //todo dz проверить вставлена ли карта
        // todo dz проверить можно ли снять деньги
        if (currentCard != null) {
            if (currentCard.getBalance()>=amount) {
                currentCard.withdraw(amount);
            } else {
                System.out.println("Недостаточно средств");
            }
        } else {
            System.out.println("Карта не вставлена");
        }

    }
}
