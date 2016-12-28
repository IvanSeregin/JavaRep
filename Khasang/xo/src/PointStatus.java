/**
 * Created by NortT on 27.12.2016.
 */
public class PointStatus
{
    final private String empty = "_";
    final private String X = "X";
    final private String O = "O";

    private String status;

    public PointStatus(String status)
    {
        this.status = status;
    }

    public String getPointStatus()
    {
        return status;
    }

    public void setPointStatus(String status)
    {
        this.status = status;
    }
}
