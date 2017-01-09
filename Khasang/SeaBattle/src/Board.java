import java.util.ArrayList;
import java.util.List;

/**
 * Created by ik34-admin on 09.01.2017.
 */
public class Board {
    List<Ship> shipList = new ArrayList<>();
    List<Point> board = new ArrayList<>();

    public Board(){
        initBoard();
        //trying to create a 4-deck ship
        int count = 0;
        ShipFactory shipFactory = new ShipFactory();

        while (count != Consts.FOUR_DECK_COUNT){
            Ship ship = shipFactory.createShip(Consts.FOUR_DECK_SIZE);
            if (isFit(ship)){
                fitShip(ship);
                count ++;
            }
        }
    }

    public Point getPoint(Coordinate coordinate){
        return board.get(coordinate.getX()*Consts.BOARD_WIDTH + coordinate.getY());
    }

    private void initBoard() {
        for (int i=0; i<Consts.BOARD_WIDTH; i++){
            for (int j = 0; j<Consts.BOARD_HEIGHT; j++){
                board.add(i*Consts.BOARD_WIDTH + j, new Point(new Coordinate(i ,j)));
            }
        }
    }

    /*
    check whether we can fit a new ship into the board or not
    whether we can do it or not depends on the size and the
    position (horizontal or vertical)
     */
    private boolean isFit(Ship ship){
        if (ship.isHorizontal()){
            if(ship.getCoordinate().getX() <= Consts.BOARD_WIDTH-ship.getSize()){
                return true;
            }
            else {
                return false;
            }
        }
        else{
            if(ship.getCoordinate().getY() <= Consts.BOARD_HEIGHT -ship.getSize()){
                return true;
            }
            else {
                return false;
            }
        }
    }

    private void fitShip(Ship ship) {
        Point point = new Point(ship.getCoordinate());
        point.setStatus(PointStatus.UNDAMAGED_SHIP);

        if (ship.isHorizontal()){
            for (int i = 0; i<ship.getSize(); i++){
                int n = point.getCoordinate().getX()*Consts.BOARD_WIDTH + point.getCoordinate().getY() + i;
                this.board.set(n, point);
            }
        }
        else{
            for (int i = 0; i<ship.getSize(); i++){
                int n = (point.getCoordinate().getY()+i)*Consts.BOARD_HEIGHT + point.getCoordinate().getX();
                this.board.set(n, point);
            }
        }
    }

}
