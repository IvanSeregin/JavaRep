/**
 * Created by NortT on 22.01.2017.
 */
public class HumanPlayer extends AbstractPlayer {
    Displayable display;

    public void initWithDisplay(Displayable display){
        this.display = display;
    }

    @Override
    public boolean shoot() {
        Coordinate coordinate;
        do {
            coordinate = display.askForShot();
        } while (board.getPoint(coordinate).getStatus() == PointStatus.SANK_SHIP ||
                board.getPoint(coordinate).getStatus() == PointStatus.DAMAGED_SHIP ||
                board.getPoint(coordinate).getStatus() == PointStatus.MISSED);

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
