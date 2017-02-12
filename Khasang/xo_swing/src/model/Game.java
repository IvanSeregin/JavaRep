package model; /**
 * Created by NortT on 27.12.2016.
 */
import controller.GameController;
import view.Displayable;

import java.util.Random;

public class Game implements Runnable
{
    static Game instance;
    private static GameController controller = GameController.getInstance();

    private Board board;
    private Player playerX;
    private Player playerO;

    private Game(){} //we use singleton here

    public void init(Displayable display)
    {
        controller.initWithDisplay(display);
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
        controller.displayBoard(board);
    }

    public void showWinner()
    {
        if(playerO.isWinner())
        {
            controller.showWinner(playerO);
        } else if(playerX.isWinner()) {
            controller.showWinner(playerX);
        } else
            controller.showWinner();
    }

    public void start()
    {
        while(!board.isFull())
        {
            //It's X'x turn
            //X is trying to set the point to a free square on the board
            controller.displayBoard(board);
            while (!board.setPoint(playerX.turn()) && !board.isFull());
            {
                if (board.isTheEnd())
                {
                    playerX.setWinner(true);
                    controller.displayBoard(board);
                    this.showWinner();
                    return;
                }
            }

            controller.displayBoard(board);
            //It's O's turn
            while (!board.setPoint(playerO.turn()) && !board.isFull());
            {
                if (board.isTheEnd())
                {
                    playerO.setWinner(true);
                    controller.displayBoard(board);
                    this.showWinner();
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
        controller.greetPlayers(playerX, playerO);
    }

    @Override
    public void run() {
        this.start();
    }
}
