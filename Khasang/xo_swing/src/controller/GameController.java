package controller;

import model.*;
import view.Displayable;

/**
 * Created by ik34-admin on 07.02.2017.
 */
public class GameController {
    private static Displayable display;

    public static void doShoot(Point point) {
        HumanPlayer.setPoint(point);
        HumanPlayer.setIsShoot(true);
    }

    public static String getPlayerName() {
        return display.getPlayerName();
    }

    public static void initWithDisplay(Displayable d) {
        display = d;
    }

    public static void greetPlayers (Player playerX, Player playerO) {
        display.greetPlayers(playerX.getName(), playerO.getName());
    }

    public static void showWinner (Player player) {
        display.showWinner(player.getName());
    }

    public static void showWinner() {
        display.showWinner();
    }

    public static void displayBoard(Board board) {
        display.displayBoard(board);
    }

    public static void newGame() {
        Main.newGame();
    }
}
