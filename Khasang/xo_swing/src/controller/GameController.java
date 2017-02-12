package controller;

import model.*;
import view.Displayable;

/**
 * Created by ik34-admin on 07.02.2017.
 */
public class GameController {
    private Displayable display;
    private static GameController instance;

    private GameController () {}

    public static GameController getInstance() {
        return instance;
    }

    public static void setInstance(GameController instance1) {
        instance = instance1;
    }

    public void doShoot(Point point) {
        HumanPlayer.setPoint(point);
        HumanPlayer.setIsShoot(true);
    }

    public String getPlayerName() {
        return display.getPlayerName();
    }

    public void initWithDisplay(Displayable d) {
        display = d;
    }

    public void greetPlayers (Player playerX, Player playerO) {
        display.greetPlayers(playerX.getName(), playerO.getName());
    }

    public void showWinner (Player player) {
        display.showWinner(player.getName());
    }

    public void showWinner() {
        display.showWinner();
    }

    public void displayBoard(Board board) {
        display.displayBoard(board);
    }

    public void newGame() {
        Main.newGame();
    }
}
