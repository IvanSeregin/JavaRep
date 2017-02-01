import java.io.*;

/**
 * Created by igor on 14.08.16.
 */
public class Main {
    public static void main(String[] args) {

        class Cat implements Serializable{
            String name;

            public Cat(){};

            public Cat(String name){
                this.name = name;
            }

            @Override
            public String toString() {
                return name;
            }
        }

        Cat cat = new Cat("Barsik");

        try (ObjectOutputStream out = new ObjectOutputStream(new BufferedOutputStream (new FileOutputStream("cat.txt", false)))){
            out.writeObject(cat);
        }
        catch (IOException e){
            e.printStackTrace();
        }

        Cat cat1 = new Cat();
        try (ObjectInputStream in = new ObjectInputStream(new BufferedInputStream(new FileInputStream("cat.txt")))){

            cat1 = (Cat) in.readObject();
        } catch (IOException e){

        } catch (ClassNotFoundException e) {
            e.printStackTrace();
        }

        System.out.println(cat1);
    }

        /*
        Atm atm = new Atm();
        Card masterCard = new MasterCard(100);
        Card visa = new Visa(300);

        atm.insertCard(masterCard);
        atm.insertCard(visa);
        atm.eject();
        atm.insertCard(visa);

//        atm.currentCard = masterCard;
//        atm.currentCard = masterCard2;

//        masterCard.balance = 100000; // todo dz

        atm.showBalance();
        atm.withdraw(200);
        atm.showBalance();

        Visa visa1 = new Visa(300);
        atm.eject();
        atm.insertCard(visa1);
        */
}
