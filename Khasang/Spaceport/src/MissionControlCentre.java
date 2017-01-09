/**
 * Created by NortT on 01.01.2017.
 */
public class MissionControlCentre
{
    Lunchable lunchable;

    public void setLunchable(Lunchable lunchable)
    {
        this.lunchable = lunchable;
    }

    void lunch()
    {
        if (startPermitted())
        {
            lunchable.lunch();
        }
    }

    private boolean startPermitted()
    {
        for(int i=10; i>=0; i--)
        {
            System.out.println(i);
        }
        return true;
    }

    public void landLandable()
    {
        try
        {
            Landable landable = (Landable) lunchable;
            landable.landing();
            System.out.println("I've landed right now!!");
        }
        catch (ClassCastException e)
        {
            //e.printStackTrace();
            System.out.println("I'm not a landable object!!");
        }
    }
}
