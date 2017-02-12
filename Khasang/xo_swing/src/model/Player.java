package model;

import model.Point;
import model.PointFactory;
import model.PointStatus;

/**
 * Created by NortT on 27.12.2016.
 */
public abstract class Player
{
    String name;
    PointStatus sign;
    boolean isWinner;

    public Player(String name, PointStatus sign)
    {
        this.sign = sign;
        this.name = name;
    }

    public abstract Point turn();
    public abstract void readPlayerName();

    public boolean isWinner()
    {
        return isWinner;
    }

    public void setWinner(boolean winner)
    {
        isWinner = winner;
    }

    public Player(String name)
    {
        this.name = name;
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
