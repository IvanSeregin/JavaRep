/**
 * Created by ik34-admin on 09.01.2017.
 */
public enum PointStatus {
    EMPTY,
    OREOL,
    SINKED_SHIP,
    DAMAGED_SHIP,
    UNDAMAGED_SHIP;



    @Override
    public String toString(){
        if (this == EMPTY) return "_";
        if (this == OREOL) return "O";
        if (this == DAMAGED_SHIP) return "^";
        if (this == SINKED_SHIP) return "x";
        if (this == UNDAMAGED_SHIP) return "@";
        return "?";
    }
}
