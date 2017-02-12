package model; /**
 * Created by NortT on 27.12.2016.
 */
import java.util.Random;
public class Point
{

    private int x;
    private int y;
    private PointStatus status;

    public Point(){}

    public Point(int x, int y)
    {
        setX(x);
        setY(y);
    }

    public static Point getRandomPoint()
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

    public void setPointStatus(PointStatus status)
    {
        this.status = status;
    }

    public boolean isEmpty()
    {
        if (this.status == PointStatus.EMPTY)
        {
            return true;
        }
        return false;
    }

    public PointStatus getPointStatus()
    {
        return status;
    }
/*
    @Override
    public String toString() {
        return ("x="+x+"; y="+y+"; "+status);
    }
    */
}
