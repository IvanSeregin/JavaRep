/**
 * Created by ik34-admin on 09.01.2017.
 */
public class ConsoleDisplay implements Displayable {

    @Override
    public void readPlayers() {

    }

    @Override
    public void greetPlayers() {

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
                if (status == PointStatus.EMPTY || status == PointStatus.DAMAGED_SHIP || status == PointStatus.SINKED_SHIP)
                    System.out.print(board.getPoint(coordinate).getStatus() + "  ");
                else
                    System.out.print(PointStatus.EMPTY + "  ");
            }
            System.out.println();
        }
    }

    @Override
    public void showBoardInDebugMode(Board board) {
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
                if (board.getPoint(coordinate).getStatus() == PointStatus.UNDAMAGED_SHIP)
                    System.out.print(board.getPoint(coordinate).getStatus() + "  ");
                else
                    System.out.print(PointStatus.EMPTY + "  ");
            }
            System.out.println();
        }
    }
}
