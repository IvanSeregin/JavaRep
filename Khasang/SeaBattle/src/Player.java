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
    }

    private Board board;
    private Integer sinkCount = 0;
    private String name = "Bond";
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
            /* if currentShip is not null then we need to find appropriate point
             near the last damaged point and determine position of the ship
             (vertical or horizontal)
              */
            do {
                coordinate = Coordinate.getRandom();
            } while (board.getPoint(coordinate).getStatus() == PointStatus.SANK_SHIP ||
                    board.getPoint(coordinate).getStatus() == PointStatus.DAMAGED_SHIP ||
                    board.getPoint(coordinate).getStatus() == PointStatus.MISSED ||
                    board.getPoint(coordinate).getStatus() == PointStatus.HALO);
        }

        if (board.getPoint(coordinate).getStatus() == PointStatus.UNDAMAGED_SHIP){
            board.getPoint(coordinate).setStatus(PointStatus.DAMAGED_SHIP);
            currentShip.ship = board.getShip(board.getPoint(coordinate));
            if (currentShip == null){
                throw new NullPointerException("There is no such a ship!");
            }
            if (currentShip.ship.isSank()){
                sinkCount++;
                board.setHalo(currentShip.ship);
                currentShip.ship = null;
            }

            return true;
        }

        if (board.getPoint(coordinate).getStatus() != PointStatus.SANK_SHIP &&
                board.getPoint(coordinate).getStatus() != PointStatus.DAMAGED_SHIP){
            board.getPoint(coordinate).setStatus(PointStatus.MISSED);
        }
        return false;
    }
}
