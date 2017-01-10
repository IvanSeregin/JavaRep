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

        //place 4-deck ships on the board
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
                board.add(i*Consts.BOARD_WIDTH + j, new Point(new Coordinate(i, j)));
            }
        }
    }

    /*
    Check whether we can fit a new ship into the board or not.
    Whether we can do it or not depends on the size and the
    position (horizontal or vertical)
     */
    private boolean isFit(Ship ship){
        if (ship.isHorizontal()){
            if(ship.getHead().getCoordinate().getX() <= Consts.BOARD_WIDTH - ship.getSize()){
                //If we here, then we can fit a ship
                //Now we have to check if the place is free for a new ship
                for (int i=0; i<ship.getSize(); i++){
                    Point point = board.get(ship.getPoint(i).getCoordinate().toLinear());
                    //if another ship is located in this point, return false
                    if (point.getStatus() != PointStatus.EMPTY) {
                        return false;
                    }
                    else {
                        //if this point is empty so we have to check neighbours points
                        //to avoid two ships are placed together
                        if (!isHaloSuitable(point)) {
                            return false;
                        }
                    }
                }
                return true;
            }
            else {
                return false;
            }
        }
        else{
            if(ship.getHead().getCoordinate().getY() <= Consts.BOARD_HEIGHT -ship.getSize()){
                //If we here, then we can fit a ship
                //Now we have to check if the place is free for a new ship
                for (int i=0; i<ship.getSize(); i++){
                    Point point = board.get(ship.getPoint(i).getCoordinate().toLinear());
                    if (point.getStatus() != PointStatus.EMPTY) {
                        return false;
                    }
                    else {
                        //if this point is empty so we have to check neighbours points
                        //to avoid two ships are placed together
                        if (!isHaloSuitable(point)) {
                            return false;
                        }
                    }
                }
                return true;
            }
            else {
                return false;
            }
        }
    }

    private boolean isHaloSuitable(Point point) {
        /*The halo of a point consists of 8 points which surround the point
            123
            8*4
            765
            In this example * is the source point and points 12345678 are halo of the point
            If the halo is an empty point or a halo of another ship, it's fine, we can place the
             ship near the point
         */
        for (int i=-1; i<=1; i++){
            for (int j=-1; j<=1; j++){
                Coordinate coord = new Coordinate(
                        point.getCoordinate().getX()+i,
                        point.getCoordinate().getY()+j);
                if (coord.isValid()) {
                    if (board.get(point.getCoordinate().toLinear()).getStatus() != PointStatus.EMPTY
                            && board.get(point.getCoordinate().toLinear()).getStatus() != PointStatus.OREOL) {
                        return false;
                    }
                }
            }
        }
        return true;
    }

    private void fitShip(Ship ship) {
        //this method substitutes points of the board for point of the ship
        //so when we change (damage for example) a ship these changes are shown on the board
        //and vice versa
        for (int i = 0; i<ship.getSize(); i++){
            Point point = ship.getPoint(i);
            this.board.set(point.getCoordinate().toLinear(), point);
        }

        setHalo(ship);
    }

    //We use the method setHalo() to avoid two ships together
    private void setHalo(Ship ship) {
        //TODO here
    }

}
