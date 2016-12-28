/**
 * Created by ik34-admin on 28.12.2016.
 */
public class ConsoleDisplay implements Displayable
{
    @Override
    public void displayWinner(Game game)
    {

    }

    @Override
    public void displayBoard(Game game)
    {
        for(int i=0; i<game.getBoard().getDimension(); i++)
        {
            for (int j=0; j<game.getBoard().getDimension(); j++)
            {
                System.out.print(game.getBoard().getPoint(i, j).getPointStatus() + " ");
            }
            System.out.println("");
        }
        System.out.println("");
    }

    @Override
    public void readPlayerName()
    {

    }

    @Override
    public void showWinner(Player player)
    {

    }
}
