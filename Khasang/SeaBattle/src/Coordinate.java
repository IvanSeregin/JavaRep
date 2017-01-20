import java.util.Random;

/**
 * Created by ik34-admin on 09.01.2017.
 */
public class Coordinate {
    private int d, l;

    public Coordinate(){}

    public Coordinate(int d, int l) {
        this.d = d;
        this.l = l;
    }


    public int getD() {
        return d;
    }

    public void setD(int d) {
        this.d = d;
    }

    public int getL() {
        return l;
    }

    public void setL(int l) {
        this.l = l;
    }

    public void setDL(int x, int y) {
        this.d = x;
        this.l = y;
    }

    public static Coordinate getRandom() {
        Random randomX = new Random();
        Random randomY = new Random();
        return new Coordinate(randomX.nextInt(Consts.BOARD_WIDTH), randomY.nextInt(Consts.BOARD_HEIGHT));
    }

    public int toLinear() {
        return this.d *Consts.BOARD_WIDTH + this.l;
    }


    public boolean isValid() {
        return ((d >= 0 && d <Consts.BOARD_WIDTH) && (l >= 0 && l <Consts.BOARD_HEIGHT));
    }

    @Override
    public String toString(){
        return d + " " + l;
    }
}
