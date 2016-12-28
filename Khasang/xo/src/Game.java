/**
 * Created by NortT on 27.12.2016.
 */
import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.IOException;

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
        try
        {
            BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
            System.out.println("Hello, X player! Write yur name, pls: ");
            String name = br.readLine();
            playerX.setName(name);
            System.out.println("Hello, O player! Write yur name, pls: ");
            name = br.readLine();
            playerO.setName(name);
            System.out.println("Hello, " + playerX + " and " + playerO);
        }
        catch (IOException e)
        {
            e.printStackTrace();
        }
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

    public Player getWinner()
    {
        //void showWinner(Player player);
        if(playerO.isWinner())
        {
            return playerO;
        }
        if(playerX.isWinner())
        {
            return playerX;
        }

        return null;
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
                    board.show();
                    return;
                }
            }
            //It's O's turn
            while (!board.setPoint(playerO.turn()) && !board.isFull());
            {
                if (board.isTheEnd())
                {
                    playerO.setWinner(true);
                    board.show();
                    return;
                }
            }
            //displayBoard;
            board.show();
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
