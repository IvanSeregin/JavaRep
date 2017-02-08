package model;

import model.*;
import view.GraphicsDisplay;

/**
 * Created by NortT on 27.12.2016.
 */
public class Main
{
    public static void main(String[] args)
    {
        Game game = Game.getInstance();
        Player playerX = new CompPlayer("James Bond", PointStatus.X);
        Player playerO = new HumanPlayer("Looser", PointStatus.O);

        game.initWithBoard(new Board());
        game.initWithPlayers(playerX, playerO);
        game.init(new GraphicsDisplay());
        game.start();
        game.showWinner();
    }
}
