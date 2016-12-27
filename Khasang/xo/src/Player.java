/**
 * Created by ik34-admin on 27.12.2016.
 */
public class Player
{
    String name;
    PointStatus sign;

    public Player(PointStatus sign)
    {
        this.sign = sign;
    }

    public Point turn()
    {

        PointFactory pointFactory = new PointFactory();
        return pointFactory.createRandomPoint(sign);
    }

    public String getName()
    {
        return name;
    }

    public void setName(String name)
    {
        this.name = name;
    }

    public PointStatus getSign()
    {
        return sign;
    }

    public void setSign(PointStatus sign)
    {
        this.sign = sign;
    }

    @Override
    public String toString()
    {
        return getName();
    }



}
