package controller;

import model.*;
import view.ConsoleDisplay;
import view.Displayable;
import view.GraphicsDisplay;

/**
 * Created by ik34-admin on 07.02.2017.
 */
public class GameController implements Runnable{



    public enum PlayerType {
        USER("User"), PC("Computer"), NETWORK ("Network"), EMPTY("Empty");

        PlayerType(String empty) {

        }
    }

    private Displayable display; //view
    private Game gameInstance; //model
    private static GameController instance; //controller

    private volatile Point point;
    private volatile PlayerType playerType=PlayerType.EMPTY;

    public static Object key = new Object();

    private GameController () {}

    public void init()
    {
        this.gameInstance = Game.getInstance();
        this.display = GraphicsDisplay.getInstance();
    }

    public static GameController getInstance() {
        if (instance == null)
            instance = new GameController();
        return instance;
    }

    public void doShoot(Point point) {
        setPoint(point);
        synchronized (key) {
            key.notifyAll();
        }
    }

    public String getPlayerName() {
        return display.getPlayerName();
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

    public void setShoot(boolean shot) {
        synchronized (key) {
            key.notifyAll();
        }
    }

    public Point getPoint() {
        return point;
    }

    public void setPoint(Point point) {
        this.point = point;
    }

    public void setPlayerType(PlayerType playerType) {
        this.playerType = playerType;
        synchronized (key) {
            key.notifyAll();
        }
    }

    public PlayerType determinePlayerType() {
        display.getPlayerType();
        return playerType;
    }

    public PlayerType getPlayerType() {
        return playerType;
    }

    public int getPlayerCount() {
        return gameInstance.getPlayerCount()+1;
    }

    @Override
    public void run() {
        gameInstance.run();
    }

}
