/**
 * Created by ik34-admin on 27.12.2016.
 */
public class Game
{
    private Game game;
    private Board board;
    private Game(){}

    public Game init()
    {
        if (game == null)
        {
            game = new Game();
        }
        return game;
    }

    public void initWithBoard(Board board)
    {
        this.board = board;
    }
}
