import java.util.Random;

/**
 * Created by NortT on 24.01.2017.
 * This class generates ships on the edge of the board
 */
public class SmartShipFactory extends AbstractShipFactory {

    @Override
    public Ship createShip(int size) {
        Random random = new Random();
        boolean vertical;
        Integer d, l;
        d = random.nextInt(Consts.BOARD_WIDTH);
        if (d == 0 || d == Consts.BOARD_WIDTH-1){
            vertical = false;
            l = random.nextInt(Consts.BOARD_WIDTH);
        }
        else {
            vertical = true;
            l = (random.nextInt(100) >= 50) ? 0 : Consts.BOARD_HEIGHT-1;
        }


        Coordinate coordinate = new Coordinate(d, l);
        Ship ship = new Ship(size, coordinate, vertical);
        return ship;
    }
}
