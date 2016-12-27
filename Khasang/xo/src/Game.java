/**
 * Created by ik34-admin on 27.12.2016.
 */
public class Game
{
    static Game instance;
    private Board board;
    private Player playerX;
    private Player playerO;

    private Game(){}

    public void init()
    {
        System.out.println("Hello, " + playerX + " and " + playerO);
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

    public void getWinner()
    {
        System.out.println("Congrats!");
    }

    public void start()
    {


        while(!board.isFull())
        {
            while (!board.setPoint(playerX.turn()) && !board.isFull());
            {
            }
            while (!board.setPoint(playerO.turn()) && !board.isFull());
            {
            }
            board.show();
        }
    }

    public void initWithPlayers(Player playerX, Player playerO)
    {
        this.playerO = playerO;
        this.playerX = playerX;
    }
}
