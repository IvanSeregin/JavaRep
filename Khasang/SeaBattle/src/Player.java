/**
 * Created by ik34-admin on 09.01.2017.
 */
public class Player {
    private enum PlayerStatus{
        HUMAN,
        PC
    }

    private enum ShipPosition{
        VERTICAL,
        HORIZONTAL,
        NONE
    }

    private class CurrentShip{
        public Ship ship;
        public ShipPosition shipPosition;
        public Integer damagedPoints;
        public Point lastDamagedPoint;
        public Point firstDamagedPoint;
        public ShipPosition tmpShipPosition;

        CurrentShip(){
            ship = null;
            shipPosition = ShipPosition.NONE;
            damagedPoints = 0;
            lastDamagedPoint = null;
            firstDamagedPoint = null;
            tmpShipPosition = ShipPosition.NONE;
        }

        private boolean isValidToShot(Coordinate coordinate){
            if (coordinate.isValid()) {
                if (board.getPoint(coordinate).getStatus() == PointStatus.EMPTY ||
                        board.getPoint(coordinate).getStatus() == PointStatus.UNDAMAGED_SHIP) {
                    return true;
                }
            }
            return  false;
        }
        /*
        This method is called when the position of the ship is not known yet.
        The method returns points lying around the last damaged point.
         The Direction of checking is the following: left - top - right - bottom.

         */
        private Coordinate findNextPoint(){
            Coordinate coordinate = new Coordinate();
            tmpShipPosition = ShipPosition.NONE;
            coordinate.setDL(currentShip.lastDamagedPoint.getCoordinate().getD(),
                    currentShip.lastDamagedPoint.getCoordinate().getL()-1);
            if (currentShip.isValidToShot(coordinate)){
                tmpShipPosition = ShipPosition.HORIZONTAL;
            } else {

                coordinate.setDL(currentShip.lastDamagedPoint.getCoordinate().getD() - 1,
                        currentShip.lastDamagedPoint.getCoordinate().getL());
                if (currentShip.isValidToShot(coordinate)) {
                    tmpShipPosition = ShipPosition.VERTICAL;
                } else {

                    coordinate.setDL(currentShip.lastDamagedPoint.getCoordinate().getD(),
                            currentShip.lastDamagedPoint.getCoordinate().getL() + 1);
                    if (currentShip.isValidToShot(coordinate)) {
                        tmpShipPosition = ShipPosition.HORIZONTAL;
                    } else {

                        coordinate.setDL(currentShip.lastDamagedPoint.getCoordinate().getD() + 1,
                                currentShip.lastDamagedPoint.getCoordinate().getL());
                        if (currentShip.isValidToShot(coordinate)) {
                            tmpShipPosition = ShipPosition.VERTICAL;
                        }
                    }
                }
            }
            return coordinate;
        }

        public Coordinate findNextVerticalPoint() {
            Coordinate coordinate = probeTopAndBottom();

            if (coordinate == null) {
                //If left and right points are not suitable to shot, then return to the first damaged point
                lastDamagedPoint = firstDamagedPoint;
                coordinate = probeTopAndBottom();

                if (coordinate != null){
                    return coordinate;
                }
            }
            else {
                return coordinate;
            }

            //If something goes wrong...
            return Coordinate.getRandom();
        }

        public Coordinate findNextHorizontalPoint() {
            Coordinate coordinate = probeLeftAndRight();

            if (coordinate == null) {
                //If left and right points are not suitable to shot, then return to the first damaged point
                lastDamagedPoint = firstDamagedPoint;
                coordinate = probeLeftAndRight();

                if (coordinate != null){
                    return coordinate;
                }
            }
            else {
                return coordinate;
            }

            //If something goes wrong...
            return Coordinate.getRandom();
        }

        private Coordinate probeLeftAndRight(){
            Coordinate coordinate = new Coordinate();
            //Try to shot in the left point
            coordinate.setDL(currentShip.lastDamagedPoint.getCoordinate().getD(),
                    currentShip.lastDamagedPoint.getCoordinate().getL() - 1);
            if (currentShip.isValidToShot(coordinate)) {
                return coordinate;
            }

            //Try to shot in the right point
            coordinate.setDL(currentShip.lastDamagedPoint.getCoordinate().getD(),
                    currentShip.lastDamagedPoint.getCoordinate().getL() + 1);
            if (currentShip.isValidToShot(coordinate)) {
                return coordinate;
            }

            return null;
        }

        private Coordinate probeTopAndBottom(){
            Coordinate coordinate = new Coordinate();
            coordinate.setDL(currentShip.lastDamagedPoint.getCoordinate().getD() - 1,
                    currentShip.lastDamagedPoint.getCoordinate().getL());
            if (currentShip.isValidToShot(coordinate)) {
                return coordinate;
            }

            coordinate.setDL(currentShip.lastDamagedPoint.getCoordinate().getD() + 1,
                    currentShip.lastDamagedPoint.getCoordinate().getL());
            if (currentShip.isValidToShot(coordinate)) {
                return coordinate;
            }

            return null;
        }
    }

    private Board board;
    private Integer sinkCount = 0;
    private String name = "Bond"; //default name
    private PlayerStatus status;
    CurrentShip currentShip = new CurrentShip();


    public Player() {
        status = PlayerStatus.PC;
    }

    public void initWithBoard(Board board) {
        this.board = board;
    }

    public boolean isWinner() {
        if (sinkCount == Consts.SHIP_COUNT)
            return true;
        return false;
    }

    public Board getBoard() {
        return board;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    /*
    This method return true if a player damaged a ship
    If a ship has been sank, then we increment the number of sank ships
    */
    public boolean  shoot(){
        Coordinate coordinate;
        if (currentShip.ship == null){
            /*
            check the status of the point with the given coordinate
            if the point has status SANK_SHIP or DAMAGED_SHIP
            then we ne to update coordinate
            */
            do {
                coordinate = Coordinate.getRandom();
            } while (board.getPoint(coordinate).getStatus() == PointStatus.SANK_SHIP ||
                    board.getPoint(coordinate).getStatus() == PointStatus.DAMAGED_SHIP ||
                    board.getPoint(coordinate).getStatus() == PointStatus.MISSED ||
                    board.getPoint(coordinate).getStatus() == PointStatus.HALO);

        }
        else {
            /* if currentShip is not null then we need to find an appropriate point
            near the last damaged point and determine the position of the ship
            (vertical or horizontal)
            */
            /*TODO: change this behaviour.
             0. Check whether the position (vertical or horizontal) of the ship is known or not.
             If the position is not known
             1. Find an empty point near the last damaged point
             2. Shot in the point
             3. If missed do nothing, in other case set the position to horizontal or vertical

             If the position is known
             1. Shot in the left or right (top or bottom) points
              */
            do {
                if (currentShip.shipPosition == ShipPosition.VERTICAL) {
                    //coordinate = Coordinate.getRandom();
                    coordinate = currentShip.findNextVerticalPoint();
                }
                else if (currentShip.shipPosition == ShipPosition.HORIZONTAL) {
                    coordinate = currentShip.findNextHorizontalPoint();
                } else {
                    coordinate = currentShip.findNextPoint();
                }


            } while (board.getPoint(coordinate).getStatus() == PointStatus.SANK_SHIP ||
                    board.getPoint(coordinate).getStatus() == PointStatus.DAMAGED_SHIP ||
                    //board.getPoint(coordinate).getStatus() == PointStatus.UNDAMAGED_SHIP ||
                    board.getPoint(coordinate).getStatus() == PointStatus.MISSED ||
                    board.getPoint(coordinate).getStatus() == PointStatus.HALO);
        }

        /*
        If the given coordinate points to a point with the status UNDAMAGED_SHIP
        this means we damage a ship and we have to change the status of the point.
        Also we have to check whether the ship was sank or not. If it's so we increment counter of sank ships
        and set halo around sank ship to avoid unnecessary shots and reset the current ship
        */
        if (board.getPoint(coordinate).getStatus() == PointStatus.UNDAMAGED_SHIP){
            board.getPoint(coordinate).setStatus(PointStatus.DAMAGED_SHIP);
            if (currentShip.firstDamagedPoint == null){
                currentShip.firstDamagedPoint = board.getPoint(coordinate);
            }
            currentShip.lastDamagedPoint = board.getPoint(coordinate);
            currentShip.damagedPoints ++;
            currentShip.ship = board.getShip(board.getPoint(coordinate));
            if (currentShip.shipPosition == ShipPosition.NONE){
                currentShip.shipPosition = currentShip.tmpShipPosition;
            }
            if (currentShip.ship == null){
                throw new NullPointerException("There is no such a ship!");
            }
            if (currentShip.ship.isSank()){
                sinkCount++;
                board.setHalo(currentShip.ship);
                currentShip = new CurrentShip();
            }

            return true;
        }
        /*
        If the given coordinate points to a point with the status which is different than SANK_SHIP or DAMAGED_SHIP
        then we mark the point as MISSED.
        */
        if (board.getPoint(coordinate).getStatus() != PointStatus.SANK_SHIP &&
                board.getPoint(coordinate).getStatus() != PointStatus.DAMAGED_SHIP){
            board.getPoint(coordinate).setStatus(PointStatus.MISSED);
        }
        return false;
    }
}
