/**
 * Created by ik34-admin on 09.01.2017.
 */
public class Player {
    private enum PlayerStatus{
        HUMAN,
        PC
    }

    private Board board;
    private Integer sinkCount = 0;
    private String name = "Bond";
    private PlayerStatus status;

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

    //This method return true if a player has damaged a ship
    //If a ship has been damaged, then we increment the number of sank ships
    public boolean  shoot(){
        Coordinate coordinate = Coordinate.getRandom();
        if (board.getPoint(coordinate).getStatus() == PointStatus.UNDAMAGED_SHIP){
            board.getPoint(coordinate).setStatus(PointStatus.DAMAGED_SHIP);
            Ship ship = board.getShip(board.getPoint(coordinate));
            if (ship == null){
                throw new NullPointerException("There is no such a ship!");
            }
            if (ship.isSank()){
                sinkCount++;
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
