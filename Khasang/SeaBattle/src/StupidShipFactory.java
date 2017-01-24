import java.util.Random;

/**
 * Created by NortT on 24.01.2017.
 * This class generates a ship with a random head with a random position (vertical or horizontal)
 */
public class StupidShipFactory extends AbstractShipFactory {
    @Override
    public Ship createShip(int size) {
        Random random = new Random();
        Coordinate coordinate = Coordinate.getRandom();
        Ship ship = new Ship(size, coordinate, random.nextBoolean());
        return ship;
    }
}
