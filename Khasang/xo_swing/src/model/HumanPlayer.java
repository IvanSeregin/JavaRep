package model;

import controller.GameController;

import java.util.Random;

/**
 * Created by ik34-admin on 07.02.2017.
 */
public class HumanPlayer extends Player {

    private volatile static Point point;
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
        //Тут программа зацикливается при запуске через пункт Новая игра, ожидая хода игрока
        //но игрок сходить не может, т.к. ГУИ висит
        isShot = false;
        while (!isShot);
        point.setPointStatus(this.getSign());
        return point;
    }

    @Override
    public void readPlayerName() {
        String name = GameController.getPlayerName();
        this.setName(name);
    }

    public static void setIsShoot(boolean b) {
        isShot = b;
    }
}
