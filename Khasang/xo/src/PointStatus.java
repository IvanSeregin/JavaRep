/**
 * Created by NortT on 05.01.2017.
 */
public enum PointStatus {
    EMPTY,
    X,
    O;

    @Override
    public String toString()
    {
        if (this == EMPTY)
            return "_";
        else if (this == X)
            return "X";
        return "O";

    }
}
