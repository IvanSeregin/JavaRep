/**
 * Created by ik34-admin on 09.01.2017.
 */
public class Main {
    public static void main(String[] args) {
        Game game = Game.getInstance();
        Player player1 = new Player();
        Player player2 = new Player();
        Displayable display = new ConsoleDisplay();

        Board board1 = new Board();
        //display.showBoardInDebugMode(board1);
        display.showBoard(board1);

        Board board2 = new Board();
        display.showBoard(board2);

        player1.initWithBoard(board1);
        player2.initWithBoard(board2);

        game.initWithDisplay(display);
        game.initWithPlayers(player1, player2);
    }
}
