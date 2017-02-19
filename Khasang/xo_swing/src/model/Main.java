package model;

import controller.GameController;

/**
 * Created by NortT on 27.12.2016.
 */
public class Main
{
    private static GameController gameController = GameController.getInstance();
    public static void main(String[] args)
    {
        gameController.init();
        newGame();
    }

    /**
     * What's JavaDoc? :)
     * This static method is called every time a user wants to run a new game.
     * Each new game has own thread
     * What happened with threads already running???
     * Are they alive???
     * Should I stop the old threads or not???
     *
     * @see Thread
     * @see GameController
     */
    public static void newGame() {
        new Thread(gameController).start();
    }
}
