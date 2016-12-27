/**
 * Created by ik34-admin on 27.12.2016.
 */
import java.util.Random;
public class Point
{
    private int x;
    private int y;
    private Status status;

    public Point(){}

    public Point(int x, int y)
    {
        setX(x);
        setY(y);
    }

    public Point getRandomPoint()
    {
        Random random = new Random();
        Point point = new Point(random.nextInt(3), random.nextInt(3));
        return point;
    }

    public int getX()
    {
        return x;
    }

    public void setX(int x)
    {
        this.x = x;
    }

    public int getY()
    {
        return y;
    }

    public void setY(int y)
    {
        this.y = y;
    }

    @Override
    public String toString()
    {
        return "X: " + x + "; Y: " + y;
    }

    public Status getStatus()
    {
        return status;
    }

    public void setStatus(Status status)
    {
        this.status = status;
    }
}
