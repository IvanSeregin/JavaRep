/**
 * Created by ik34-admin on 09.01.2017.
 */
public interface Displayable {
    void readPlayers();
    void greetPlayers();
    void showBoard(Board board);
    void showBoardInDebugMode(Board board); //This is for debug purposes only, not used in production
}
