/**
 * Created by ik34-admin on 27.12.2016.
 */
public class Main
{
    public static void main(String[] args)
    {
        Game game = Game.getInstance();
        Player playerX = new Player(new PointStatus("X"));
        Player playerO = new Player(new PointStatus("O"));
        game.init();
        game.initWithBoard(new Board());
        game.initWithPlayers(playerX, playerO);
        game.start();
        game.getWinner();
    }
}
