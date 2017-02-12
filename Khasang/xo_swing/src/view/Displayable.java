package view;

import model.Board;
import model.Game;

/**
 * Created by NortT on 28.12.2016.
 */
public interface Displayable
{
    void displayBoard(Board board);
    void showWinner(String player);
    void showWinner();
    void greetPlayers(String playerX, String playerO);
    String getPlayerName();
}
