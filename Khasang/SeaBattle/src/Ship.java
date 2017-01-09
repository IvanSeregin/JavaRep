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
    private Coordinate coordinate;
    private int size;

    public Ship(){

    }

    public Ship(Coordinate coordinate, boolean horizontal){
        this.coordinate = coordinate;
        this.horizontal = horizontal;
    }

    public boolean placeInPosition(Coordinate coordinate){
        this.coordinate = coordinate;
        return true;
    }

    public boolean isHorizontal() {
        return horizontal;
    }

    public void setHorizontal(boolean horizontal) {
        this.horizontal = horizontal;
    }

    public Coordinate getCoordinate() {
        return coordinate;
    }

    public void setCoordinate(Coordinate coordinate) {
        this.coordinate = coordinate;
    }

    public int getSize() {
        return size;
    }

    public void setSize(int size) {
        this.size = size;
    }
}
