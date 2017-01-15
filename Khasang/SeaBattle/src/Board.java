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


        //place n-deck ships on the board
        placeShip(Consts.FOUR_DECK_COUNT, Consts.FOUR_DECK_SIZE);
        placeShip(Consts.THREE_DECK_COUNT, Consts.THREE_DECK_SIZE);
        placeShip(Consts.TWO_DECK_COUNT, Consts.TWO_DECK_SIZE);
        placeShip(Consts.ONE_DECK_COUNT, Consts.ONE_DECK_SIZE);
    }

    /*
    The method places *count* ships with specified size on the board.
    ShipFactory creates a random ship, then if it's possible to place
    the ship on the board (isFit() checks it), we call fitShip() to place the ship
    and save ship to shipList, otherwise we create another ship with ShipFactory
     */
    private void placeShip(int count, int size) {
        int n = 0;
        ShipFactory shipFactory = new ShipFactory();
        while (n != count){
            Ship ship = shipFactory.createShip(size);
            if (isFit(ship)){
                fitShip(ship);
                shipList.add(ship);
                n ++;
            }
        }
    }

    //return specified point
    public Point getPoint(Coordinate coordinate){
        return board.get(coordinate.getD()*Consts.BOARD_WIDTH + coordinate.getL());
    }

    //fill the board with empty points
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
        if (ship.isVertical()){
            if(ship.getHead().getCoordinate().getD() <= Consts.BOARD_WIDTH - ship.getSize()){
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
            if(ship.getHead().getCoordinate().getL() <= Consts.BOARD_HEIGHT -ship.getSize()){
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

    /*The halo of a point consists of 8 points which surround the point
    123
    8*4
    765
    In this example * is the source point and points 12345678 are halo of the point
    If the halo is an empty point or a halo of another ship, it's fine, we can place the
     ship near the point
 */
    private boolean isHaloSuitable(Point point) {
        for (int i=-1; i<=1; i++){
            for (int j=-1; j<=1; j++){
                Coordinate coord = new Coordinate(
                        point.getCoordinate().getD()+i,
                        point.getCoordinate().getL()+j);
                if (coord.isValid()) {
                    if (board.get(point.getCoordinate().toLinear()).getStatus() != PointStatus.EMPTY
                            && board.get(point.getCoordinate().toLinear()).getStatus() != PointStatus.HALO) {
                        return false;
                    }
                }
            }
        }
        return true;
    }

    //this method substitutes points of the board for points of the ship
    //so when we shot and damage a ship these changes are reflected on the board and the ship
    private void fitShip(Ship ship) {
        for (int i = 0; i<ship.getSize(); i++){
            Point point = ship.getPoint(i);
            this.board.set(point.getCoordinate().toLinear(), point);
        }
        setHalo(ship);
    }

    //We use the method setHalo() to set halo around specified ship.
    //Halo helps to avoid two ships together
    private void setHalo(Ship ship) {
        Coordinate shipHead = ship.getHead().getCoordinate();
        if (!ship.isVertical()){
            for (int i = shipHead.getD() - 1; i < shipHead.getD() + 2; i++){
                for (int j = shipHead.getL() - 1; j < shipHead.getL() + ship.getSize() + 1; j++){
                    Coordinate c = new Coordinate();
                    c.setDL(i, j);

                    if (c.isValid() && (board.get(c.toLinear()).getStatus() != PointStatus.UNDAMAGED_SHIP)){
                        board.get(c.toLinear()).setStatus(PointStatus.HALO);
                    }
                }
            }
        }
        else{
            for (int j = shipHead.getL() - 1; j < shipHead.getL() + 2; j++){
                for (int i = shipHead.getD() - 1; i < shipHead.getD() + ship.getSize() + 1; i++){
                    Coordinate c = new Coordinate();
                    c.setDL(i, j);

                    if (c.isValid() && (board.get(c.toLinear()).getStatus() != PointStatus.UNDAMAGED_SHIP)){
                        board.get(c.toLinear()).setStatus(PointStatus.HALO);
                    }
                }
            }
        }
    }

    //Method finds a ship with a specified point in shipList
    public Ship getShip(Point point){
        Ship ship;
        for (int i=0; i<Consts.SHIP_COUNT; i++){
            ship = shipList.get(i);
            for (int j=0; j<ship.getSize(); j++){
                if (ship.getPoint(j) == point){
                    return ship;
                }
            }
        }
        return null;
    }
}
