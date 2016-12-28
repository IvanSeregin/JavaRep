/**
 * Created by NortT on 27.12.2016.
 */
public class Player
{
    String name;
    PointStatus sign;

    public boolean isWinner()
    {
        return isWinner;
    }

    public void setWinner(boolean winner)
    {
        isWinner = winner;
    }

    boolean isWinner;

    public Player(String name)
    {
        this.name = name;
    }

    public Player(String name, PointStatus sign)
    {
        this.sign = sign;
        this.name = name;
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
