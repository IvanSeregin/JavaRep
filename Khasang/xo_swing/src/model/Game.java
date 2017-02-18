package model; /**
 * Created by NortT on 27.12.2016.
 */
import controller.GameController;
import view.Displayable;

public class Game implements Runnable
{
    static Game instance;
    private static GameController gameController = GameController.getInstance();

    private Board board;
    private Player playerX;
    private Player playerO;
    private volatile int playerCount = 0;

    private Game(){} //we use singleton here

    public static Game getInstance()
    {
        if (instance == null)
        {
            instance = new Game();
        }
        return instance;
    }

    public void initWithBoard(Board board)
    {
        this.board = board;
        gameController.displayBoard(board);
    }

    public void showWinner()
    {
        if(playerO.isWinner())
        {
            gameController.showWinner(playerO);
        } else if(playerX.isWinner()) {
            gameController.showWinner(playerX);
        } else
            gameController.showWinner();
    }

    public void start()
    {
        while(!board.isFull())
        {
            //It's X'x turn
            //X is trying to set the point to a free square on the board
            gameController.displayBoard(board);
            if (!board.isFull())
                while (!board.setPoint(playerX.turn()));
                {
                    if (board.isTheEnd())
                    {
                        playerX.setWinner(true);
                        gameController.displayBoard(board);
                        this.showWinner();
                        return;
                    }
                }

            gameController.displayBoard(board);
            //It's O's turn
            if (!board.isFull())
                while (!board.setPoint(playerO.turn()));
                {
                    if (board.isTheEnd())
                    {
                        playerO.setWinner(true);
                        gameController.displayBoard(board);
                        this.showWinner();
                        return;
                    }
                }
        }
        this.showWinner();
    }

    @Override
    public void run() {
        playerCount = 0;
        Player playerX = createPlayer(PointStatus.X);
        this.playerX = playerX;

        Player playerO = createPlayer(PointStatus.O);
        this.playerO = playerO;

        this.initWithBoard(new Board());

        gameController.greetPlayers(playerX, playerO);

        this.start();
    }

    private Player createPlayer(PointStatus pointStatus) {
        Player player = null;
        playerCount ++;
        //System.out.println(gameController.getGameThread().getName());
        //while (gameController.determinePlayerType().equals(GameController.PlayerType.EMPTY)) ;

        System.out.println("Sleep");
        synchronized (GameController.key) {
            try {
                GameController.key.wait();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
        System.out.println("Awake");
        switch (gameController.getPlayerType()) {
            case PC:
                player = new CompPlayer("Looser", pointStatus);
                break;
            case USER:
                player = new HumanPlayer("Looser", pointStatus);
                break;
        }
        player.readPlayerName();

        //gameController.setPlayerType(GameController.PlayerType.EMPTY);

        return player;
    }

    public int getPlayerCount() {
        return playerCount;
    }
}
