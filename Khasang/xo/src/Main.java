/**
 * Created by NortT on 27.12.2016.
 */
public class Main
{
    public static void main(String[] args)
    {
        Game game = Game.getInstance();
        Player playerX = new Player("James Bond", PointStatus.X);
        Player playerO = new Player("Looser", PointStatus.O);

        game.initWithBoard(new Board());
        game.initWithPlayers(playerX, playerO);
        game.init(new ConsoleDisplay());
        game.start();
        game.showWinner();
    }
}
