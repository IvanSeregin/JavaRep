package controller;

import model.HumanPlayer;
import model.Point;

/**
 * Created by ik34-admin on 07.02.2017.
 */
public class GameController {
    public static void doShoot(Point point) {
        HumanPlayer.setPoint(point);
        HumanPlayer.setIsShoot(true);
    }
}
