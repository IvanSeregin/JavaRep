/**
 * Created by ik34-admin on 09.01.2017.
 */
public class Point {
    private Coordinate coordinate;
    private PointStatus status = PointStatus.EMPTY;

    public Point (Coordinate coordinate){
        this.coordinate = coordinate;
    }

    public PointStatus getStatus() {
        return status;
    }

    public void setStatus(PointStatus status) {
        this.status = status;
    }

    public Coordinate getCoordinate() {
        return coordinate;
    }

    public void setCoordinate(Coordinate coordinate) {
        this.coordinate = coordinate;
    }
}
