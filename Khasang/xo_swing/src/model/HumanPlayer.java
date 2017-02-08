package model;

/**
 * Created by ik34-admin on 07.02.2017.
 */
public class HumanPlayer extends Player {

    private static Point point;
    private volatile static boolean isShot;

    public HumanPlayer(String name, PointStatus sign) {
        super(name, sign);
    }

    public HumanPlayer(String name) {
        super(name);
    }

    public static void setPoint(Point point) {
        HumanPlayer.point = point;
    }

    @Override
    public Point turn() {
        isShot = false;
        while (!isShot);
        point.setPointStatus(this.getSign());
        return point;
    }

    public static void setIsShoot(boolean b) {
        isShot = b;
    }
}
