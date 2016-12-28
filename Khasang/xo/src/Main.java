/**
 * Created by NortT on 27.12.2016.
 */
public class Main
{
    public static void main(String[] args)
    {
        Game game = Game.getInstance();
        Player playerX = new Player("James Bond", new PointStatus("X"));
        Player playerO = new Player("Looser", new PointStatus("O"));

        game.initWithBoard(new Board());
        game.initWithPlayers(playerX, playerO);
        game.init(new ConsoleDisplay());
        game.start();
        //displayWinner
        if (game.getWinner()!= null)
            System.out.println(game.getWinner() + " is the winner");
        else
            System.out.println("Nobody is the winner");
    }
}
