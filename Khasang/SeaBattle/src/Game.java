/**
 * Created by ik34-admin on 09.01.2017.
 */
public class Game {
    private static Game instance;
    private Displayable display;
    private Player player1, player2;


    private Game(){}

    public static Game getInstance() {
        if (instance == null)
            instance = new Game();

        return instance;
    }

    public  void initWithDisplay(Displayable display) {
        this.display = display;
    }

    public void initWithPlayers(Player player1, Player player2){
        this.player1 = player1;
        this.player2 = player2;
    }

    public void start(){
        //if there is no a winner yet players shoot
        //if a player damages a ship, then he/she has the right to shoot again
        while (!player1.isWinner() && !player2.isWinner()) {
            nextTurn(player1);
            if (!player1.isWinner()) {
                nextTurn(player2);
            }
            //pause(500);
        }
        //check who is a winner
        if (player1.isWinner()){
            display.showWinner(player1);
        }
        else{
            display.showWinner(player2);
        }

        //display.showBoardInDebugMode(player1.getBoard());
        //display.showBoardInDebugMode(player2.getBoard());
    }

    private void pause(int i) {
        try {
            Thread.sleep(i);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }

    private void nextTurn(Player player){
        display.whoIsNext(player);

        while (player.shoot() && !player.isWinner()) {
            display.showBoard(player.getBoard());
        }
        display.showBoard(player.getBoard());
    }
}
