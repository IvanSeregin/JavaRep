import java.util.ArrayList;
import java.util.List;

/**
 * Created by ik34-admin on 09.01.2017.
 */
public class Ship {
    private boolean horizontal;
    /*
    coord is a coordinate of the topmost or leftmost corner of a ship
    according to its position (horizontal or not)
    I don't want to hold all coordinates of the vessel, it doesn't make sense I think.

     */
    //private Coordinate coordinate;
    List<Point> ship = new ArrayList<>();
    private int size;

    public Ship(){

    }

    public Ship(int size, Coordinate coordinate, boolean horizontal){
        for (int i=0; i<size; i++){
            Coordinate coord = new Coordinate();
            if (horizontal == true){
                coord.setX(coordinate.getX() + i);
                coord.setY(coordinate.getY());
            }
            else {
                coord.setX(coordinate.getX());
                coord.setY(coordinate.getY() + i);
            }
            Point point = new Point(coord);
            point.setStatus(PointStatus.UNDAMAGED_SHIP);
            ship.add(i, point);
        }
        this.horizontal = horizontal;
        this.size = size;
    }

    /*
    public boolean placeInPosition(Coordinate coordinate){
        this.coordinate = coordinate;
        return true;
    }

    */
    public boolean isHorizontal() {
        return horizontal;
    }

    public Point getPoint(int n) {
        if (n >= size) return null;
        return ship.get(n);
    }
    /*
    public void setCoordinate(Coordinate coordinate) {
        this.coordinate = coordinate;
    }
    */
    public int getSize() {
        return size;
    }

    public Point getHead() {
        return ship.get(0);
    }
}
