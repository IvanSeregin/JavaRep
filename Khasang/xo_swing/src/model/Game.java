package model; /**
 * Created by NortT on 27.12.2016.
 */
import controller.GameController;

public class Game implements Runnable
{
    private class UnknownUserTypeException extends Exception {
        private String message;
        public UnknownUserTypeException(String message) {
            this.message = message;
        }
        @Override
        public String toString() {
            return "UnknownUserTypeException: " + message;
        }
    }

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
        Player playerX;
        Player playerO;
        //I just played around with exceptions here
        try {
            playerX = createPlayer(PointStatus.X);
            playerO = createPlayer(PointStatus.O);
        } catch (UnknownUserTypeException unknownUserType) {
            unknownUserType.printStackTrace();
            gameController.newGame();
            return;
        }
        this.playerX = playerX;
        this.playerO = playerO;

        this.initWithBoard(new Board());

        gameController.greetPlayers(playerX, playerO);

        this.start();
    }

    private Player createPlayer(PointStatus pointStatus) throws UnknownUserTypeException {
        Player player;

        while (gameController.determinePlayerType().equals(GameController.PlayerType.EMPTY));

        switch (gameController.getPlayerType()) {
            case PC:
                player = new CompPlayer("Looser", pointStatus);
                break;
            case USER:
                player = new HumanPlayer("Looser", pointStatus);
                break;
            default:
                throw new UnknownUserTypeException("Probably, you've chosen a user type that has not been developed yet.");
        }
        player.readPlayerName();

        gameController.setPlayerType(GameController.PlayerType.EMPTY);
        playerCount ++;
        return player;
    }

    public int getPlayerCount() {
        return playerCount;
    }


}
