import java.util.Random;

/**
 * Created by ik34-admin on 09.01.2017.
 */
public class ShipFactory {
    public Ship createShip(int size) {
        Random random = new Random();
        Coordinate coordinate = Coordinate.getRandom();
        Ship ship = new Ship(size, coordinate, random.nextBoolean());
        //Coordinate c = new Coordinate(6,0);
        //Ship ship = new Ship(size, c, true);
        return ship;
    }
}
