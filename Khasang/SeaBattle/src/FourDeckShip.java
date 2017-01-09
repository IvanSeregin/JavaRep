/**
 * Created by ik34-admin on 09.01.2017.
 */
public class FourDeckShip extends Ship {
    private int count = 0;

    public FourDeckShip(Coordinate coordinate){
        this.setCoordinate(coordinate);
    }

    public int getCount() {
        return count;
    }

    public void setCount(int count) {
        this.count = count;
    }
}
