import java.util.ArrayList;
import java.util.List;

/**
 * Created by ik34-admin on 09.01.2017.
 */
public class Ship {
    private boolean vertical;

    List<Point> ship = new ArrayList<>();
    private int size;

    private enum ShipStatus{
        UNDAMAGED,
        DAMAGED,
        SINKED
    }

    public Ship(int size, Coordinate coordinate, boolean vertical){
        for (int i=0; i<size; i++){
            Coordinate coord = new Coordinate();
            if (vertical == true){
                coord.setD(coordinate.getD() + i);
                coord.setL(coordinate.getL());
            }
            else {
                coord.setD(coordinate.getD());
                coord.setL(coordinate.getL() + i);
            }
            Point point = new Point(coord);
            point.setStatus(PointStatus.UNDAMAGED_SHIP);
            ship.add(i, point);
        }
        this.vertical = vertical;
        this.size = size;
    }

    public boolean isVertical() {
        return vertical;
    }

    public Point getPoint(int n) {
        if (n >= size) return null;
        return ship.get(n);
    }

    public int getSize() {
        return size;
    }

    public Point getHead() {
        return ship.get(0);
    }

    //The method checks whether a ship sank and if ti's so
    //changes the status of the points to SANK_SHIP
    public boolean isSank() {
        int damagedPointCount = 0;
        for (int i=0; i<size; i++){
            if (this.getPoint(i).getStatus() == PointStatus.DAMAGED_SHIP){
                damagedPointCount++;
            }
        }

        if (damagedPointCount == size){
            for (int i=0; i<size; i++){
                this.getPoint(i).setStatus(PointStatus.SANK_SHIP);
            }
            return true;
        }

        return  false;
    }
}
