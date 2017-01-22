/**
 * Created by NortT on 22.01.2017.
 */
public abstract class AbstractPlayer {
    public Board board;
    public Integer sinkCount = 0;
    public String name = "Bond"; //default name

    public abstract boolean  shoot();

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
}
