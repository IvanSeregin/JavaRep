/**
 * Created by ik34-admin on 09.01.2017.
 */
public interface Displayable {
    void readPlayers(AbstractPlayer player1, AbstractPlayer player2);
    void greetPlayers(AbstractPlayer player1, AbstractPlayer player2);
    void showBoard(Board board);
    void showBoardInDebugMode(Board board); //This is for debug purposes only
    void whoIsNext(AbstractPlayer player);
    void showWinner(AbstractPlayer player);
    void debugMessage(String message);
    Coordinate askForShot();
}
