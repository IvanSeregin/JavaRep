package model; /**
 * Created by NortT on 27.12.2016.
 */
import view.Displayable;

public class Game
{
    static Game instance;

    private Board board;
    private Player playerX;
    private Player playerO;
    private Displayable display;

    private Game(){} //we use singleton here

    public void init(Displayable display)
    {
        this.display = display;
        playerX = display.readPlayerName(playerX);
        playerO = display.readPlayerName(playerO);
        display.greetPlayers(playerX, playerO);
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
    }

    public void showWinner()
    {
        if(playerO.isWinner())
        {
            display.showWinner(playerO);
        } else if(playerX.isWinner()) {
            display.showWinner(playerX);
        } else
            display.showWinner();
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
                    display.displayBoard(this);
                    return;
                }
            }
            //It's O's turn
            while (!board.setPoint(playerO.turn()) && !board.isFull());
            {
                if (board.isTheEnd())
                {
                    playerO.setWinner(true);
                    display.displayBoard(this);
                    return;
                }
            }
            //displayBoard;
            display.displayBoard(this);
        }
    }

    public void initWithPlayers(Player playerX, Player playerO)
    {
        this.playerO = playerO;
        this.playerX = playerX;
    }

    public Board getBoard()
    {
        return board;
    }
}
