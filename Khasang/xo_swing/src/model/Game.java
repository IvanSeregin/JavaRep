package model; /**
 * Created by NortT on 27.12.2016.
 */
import controller.GameController;
import view.Displayable;

import java.util.Random;

public class Game
{
    static Game instance;

    private Board board;
    private Player playerX;
    private Player playerO;

    private Game(){} //we use singleton here

    public void init(Displayable display)
    {
        GameController.initWithDisplay(display);
    }

    public static Game getInstance()
    {
        if (instance == null)
        {
            instance = new Game();
        }
        return instance;
    }

    public void initWithBoard(Board board)
    {
        this.board = board;
        GameController.displayBoard(board);
    }

    public void showWinner()
    {
        if(playerO.isWinner())
        {
            GameController.showWinner(playerO);
        } else if(playerX.isWinner()) {
            GameController.showWinner(playerX);
        } else
            GameController.showWinner();
    }

    public void start()
    {
        while(!board.isFull())
        {
            //It's X'x turn
            //X is trying to set the point to a free square on the board
            while (!board.setPoint(playerX.turn()) && !board.isFull());
            {
                if (board.isTheEnd())
                {
                    playerX.setWinner(true);
                    GameController.displayBoard(board);
                    return;
                }
            }

            GameController.displayBoard(board);
            //It's O's turn
            while (!board.setPoint(playerO.turn()) && !board.isFull());
            {
                if (board.isTheEnd())
                {
                    playerO.setWinner(true);
                    GameController.displayBoard(board);
                    return;
                }
            }
        }
    }

    public void initWithPlayers(Player playerX, Player playerO)
    {
        playerX.readPlayerName();
        playerO.readPlayerName();
        this.playerO = playerO;
        this.playerX = playerX;
        GameController.greetPlayers(playerX, playerO);
    }
}
