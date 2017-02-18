package model;

import controller.GameController;

/**
 * Created by NortT on 27.12.2016.
 */
public class Main
{
    //private static Game game;
    private static GameController gameController = GameController.getInstance();
    public static void main(String[] args)
    {
        gameController.init();
        newGame();
    }

    //эта функция повешена на пункт меню Новая игра в слушателе в GraphicsDisplay (48 строчка)
    public static void newGame() {
        new Thread(gameController).start();
    }
}
