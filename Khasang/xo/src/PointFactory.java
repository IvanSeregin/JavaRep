/**
 * Created by NortT on 27.12.2016.
 */
public class PointFactory
{
    Point point;

    public Point createRandomPoint(PointStatus status)
    {
        point = Point.getRandomPoint();
        point.setPointStatus(status);
        return point;
    }

    public Point createPoint(int x, int y, PointStatus status)
    {
        point = new Point (x, y);
        point.setPointStatus(status);
        return point;
    }
}
