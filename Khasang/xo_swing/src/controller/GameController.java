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
        USER("User"), PC("Computer"), EMPTY("Empty");

        PlayerType(String empty) {

        }
    }

    private Displayable display; //view
    private Game gameInstance; //model
    private Thread gameThread;
    private Thread displayThread;
    private static GameController instance; //controller

    private volatile boolean isShoot;
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
//        setShoot(true);
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

    public boolean isShoot() {
        return isShoot;
    }

    public void setShoot(boolean shot) {
        synchronized (key) {
            key.notifyAll();
        }
        //isShoot = shot;
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
        //System.out.println("Hey awake!");
    }

    public void determinePlayerType() {
        display.getPlayerType();

        //return playerType;
    }

    public PlayerType getPlayerType() {
        return playerType;
    }

    public int getPlayerCount() {
        return gameInstance.getPlayerCount()+1;
    }

    @Override
    public void run() {
        gameThread = new Thread(gameInstance);
        gameThread.setName("gameThread");
        gameThread.start();
        display.getPlayerType();
        display.getPlayerType();
    }

    public Thread getGameThread() {
        return gameThread;
    }
}
