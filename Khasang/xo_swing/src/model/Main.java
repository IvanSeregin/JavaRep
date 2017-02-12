package model;

import view.ConsoleDisplay;
import view.GraphicsDisplay;

/**
 * Created by NortT on 27.12.2016.
 */
public class Main
{
    private static Game game;
    public static void main(String[] args)
    {
        game = Game.getInstance();
        game.init(new GraphicsDisplay());
        //game.init(new ConsoleDisplay());
        newGame();
        /*
        если раскомментировать newGame() ниже, то игра запустится повторно без проблем
         */
        //newGame();
    }

    //эта функция повешена на пункт меню Новая игра в слушателе в GraphicsDisplay (48 строчка)
    public static void newGame() {
        Player playerX = new HumanPlayer("James Bond", PointStatus.X);
        //Player playerO = new CompPlayer("Looser", PointStatus.O);
        Player playerO = new HumanPlayer("Looser", PointStatus.O);

        game.initWithBoard(new Board());
        game.initWithPlayers(playerX, playerO);

        new Thread(game).start();
    }
}
