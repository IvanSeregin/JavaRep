import java.util.Random;

/**
 * Created by ik34-admin on 09.01.2017.
 */
public class Coordinate {
    private int x, y;

    public Coordinate(){}

    public Coordinate(int x, int y) {
        this.x = x;
        this.y = y;
    }


    public int getX() {
        return x;
    }

    public void setX(int x) {
        this.x = x;
    }

    public int getY() {
        return y;
    }

    public void setY(int y) {
        this.y = y;
    }

    public void setXY(int x, int y) {
        this.x = x;
        this.y = y;
    }

    public static Coordinate getRandom() {
        Random randomX = new Random();
        Random randomY = new Random();
        //System.out.println(randomX.nextInt(Consts.BOARD_WIDTH) + "  " + randomY.nextInt(Consts.BOARD_HEIHT));
        return new Coordinate(randomX.nextInt(Consts.BOARD_WIDTH), randomY.nextInt(Consts.BOARD_HEIGHT));
    }

    public int toLinear() {
        return this.x*Consts.BOARD_WIDTH + this.y;
    }


    public boolean isValid() {
        return ((x>0 && x<Consts.BOARD_WIDTH) && (y>0 && y<Consts.BOARD_HEIGHT));
    }
}
