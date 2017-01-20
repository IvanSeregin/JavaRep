/**
 * Created by ik34-admin on 09.01.2017.
 */
public interface Displayable {
    void readPlayers(Player player1, Player player2);
    void greetPlayers(Player player1, Player player2);
    void showBoard(Board board);
    void showBoardInDebugMode(Board board); //This is for debug purposes only
    void whoIsNext(Player player);
    void showWinner(Player player);
    void debugMessage(String message);
}
