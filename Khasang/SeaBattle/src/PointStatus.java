/**
 * Created by ik34-admin on 09.01.2017.
 */
public enum PointStatus {
    EMPTY,
    HALO,
    SANK_SHIP,
    DAMAGED_SHIP,
    UNDAMAGED_SHIP,
    MISSED;


    @Override
    public String toString(){
        if (this == EMPTY) return "_";
        if (this == HALO) return "H";
        if (this == DAMAGED_SHIP) return "^";
        if (this == SANK_SHIP) return "x";
        if (this == UNDAMAGED_SHIP) return "@";
        if (this == MISSED) return "~";
        return "?";
    }
}
