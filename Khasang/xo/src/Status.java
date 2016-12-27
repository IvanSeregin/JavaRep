/**
 * Created by ik34-admin on 27.12.2016.
 */
public class Status
{
    final private String empty = "_";
    final private String X = "X";
    final private String O = "O";

    private String status;

    public Status()
    {
        this.status = empty;
    }

    public String getStatus()
    {
        return status;
    }

    public void setStatus(String status)
    {
        this.status = status;
    }
}
