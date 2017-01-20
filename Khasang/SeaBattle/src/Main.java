/**
 * Created by ik34-admin on 09.01.2017.
 */
public class Main {
    public static void main(String[] args) {

        Game game = Game.getInstance();

        Displayable display = new ConsoleDisplay();

        Player player1 = new Player();
        Player player2 = new Player();

        display.readPlayers(player1, player2);
        display.greetPlayers(player1, player2);

        Board board1 = new Board();
        Board board2 = new Board();
        display.showBoard(board1);
        display.showBoard(board2);

        player1.initWithBoard(board2);
        player2.initWithBoard(board1);

        game.initWithDisplay(display);
        game.initWithPlayers(player1, player2);

        game.start();
    }
}
