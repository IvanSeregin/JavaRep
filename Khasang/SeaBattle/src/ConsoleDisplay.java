import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

/**
 * Created by ik34-admin on 09.01.2017.
 */
public class ConsoleDisplay implements Displayable {

    @Override
    public void readPlayers(AbstractPlayer player1, AbstractPlayer player2) {
        try
        {
            BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
            System.out.println("Hello, write yur name, pls: ");
            String name = br.readLine();
            player1.setName(name);

            System.out.println("Hello, write yur name, pls: ");
            name = br.readLine();
            player2.setName(name);
        }
        catch (IOException e)
        {
            e.printStackTrace();
        }
    }

    @Override
    public void greetPlayers(AbstractPlayer player1, AbstractPlayer player2) {
        System.out.println(player1.getName() + ", hi there!");
        System.out.println("Hey " + player2.getName() + " g'luck!");
    }

    @Override
    public void showBoard(Board board) {
        System.out.println("    A  B  C  D  E  F  G  H  I  J");

        for (int i=0; i<Consts.BOARD_WIDTH; i++){
            //Let's make it look beautiful :)
            if (i+1 < 10)
                System.out.print(" " + (i+1) + "  ");
            else
                System.out.print(i+1 + "  ");

            //Show every singe point in board
            for (int j = 0; j<Consts.BOARD_HEIGHT; j++){
                Coordinate coordinate = new Coordinate();
                coordinate.setDL(i, j);
                PointStatus status = board.getPoint(coordinate).getStatus();
                if (status == PointStatus.EMPTY ||
                        status == PointStatus.DAMAGED_SHIP ||
                        status == PointStatus.SANK_SHIP ||
                        status == PointStatus.MISSED)
                    System.out.print(board.getPoint(coordinate).getStatus() + "  ");
                else
                    System.out.print(PointStatus.EMPTY + "  ");
            }
            System.out.println();
        }
    }

    @Override
    public void showBoardInDebugMode(Board board) {
        System.out.println("   0  1  2  3  4  5  6  7  8  9");

        for (int i=0; i<Consts.BOARD_WIDTH; i++){
            //Let's make it look beautiful :)
            System.out.print(i + "  ");

            //Show every singe point in board
            for (int j = 0; j<Consts.BOARD_HEIGHT; j++){
                Coordinate coordinate = new Coordinate();
                coordinate.setDL(i, j);
                /*
                if (board.getPoint(coordinate).getStatus() == PointStatus.UNDAMAGED_SHIP)
                    System.out.print(board.getPoint(coordinate).getStatus() + "  ");
                else
                    System.out.print(PointStatus.EMPTY + "  ");
                */
                System.out.print(board.getPoint(coordinate).getStatus() + "  ");
            }
            System.out.println();
        }
    }

    @Override
    public  void whoIsNext(AbstractPlayer player){
        System.out.println("\nNow it's " + player.getName() + "'s turn");
    }

    @Override
    public  void showWinner(AbstractPlayer player){
        System.out.println("Congrats " + player.getName() + ". You won!");
    }

    @Override
    public void debugMessage(String message) {
        System.out.println(message);
    }

    @Override
    public Coordinate askForShot() {
        System.out.println("Human inputs coordinates (Random coordinates are generated...): ");
        Coordinate coordinate = Coordinate.getRandom();
        return coordinate;
    }


}
