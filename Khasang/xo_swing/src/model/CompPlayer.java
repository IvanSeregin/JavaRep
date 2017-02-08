package model;

/**
 * Created by ik34-admin on 07.02.2017.
 */
public class CompPlayer extends Player {

    public CompPlayer(String name, PointStatus sign) {
        super(name, sign);
    }

    public CompPlayer(String name) {
        super(name);
    }

    @Override
    public Point turn()
    {
        PointFactory pointFactory = new PointFactory();
        return pointFactory.createRandomPoint(sign);
    }
}
