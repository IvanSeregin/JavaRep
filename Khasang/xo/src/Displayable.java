/**
 * Created by NortT on 28.12.2016.
 */
public interface Displayable
{
    void displayBoard(Game game);
    Player readPlayerName(Player player);
    void showWinner(Player player);
    void showWinner();
    void greetPlayers(Player playerX, Player playerO);
}
