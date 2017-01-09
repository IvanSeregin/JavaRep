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

    }

}
