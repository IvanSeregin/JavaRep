package model;

import controller.GameController;

import java.util.Random;

/**
 * Created by ik34-admin on 07.02.2017.
 */
public class HumanPlayer extends Player {

    private volatile Point point;
    private static GameController controller = GameController.getInstance();

    public HumanPlayer(String name, PointStatus sign) {
        super(name, sign);
    }

    @Override
    public Point turn() {
        synchronized (GameController.key) {
            try {
                GameController.key.wait();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }

        point = controller.getPoint();
        point.setPointStatus(this.getSign());
        controller.setShoot(false);
        return point;
    }

    @Override
    public void readPlayerName() {
        String name = controller.getPlayerName();
        this.setName(name);
    }
}
