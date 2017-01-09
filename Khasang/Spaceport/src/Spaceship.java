/**
 * Created by NortT on 01.01.2017.
 */
public class Spaceship implements  Lunchable
{
    Pilot pilot;
    String name;

    public Spaceship(String name)
    {
        this.name = name;
    }

    public Pilot getPilot()
    {
        return pilot;
    }

    public void setPilot(Pilot pilot)
    {
        this.pilot = pilot;
    }

    public String getName()
    {
        return name;
    }

    public void setName(String name)
    {
        this.name = name;
    }

    @Override
    public void lunch()
    {
        System.out.println("Spaceship was lunched, " + pilot.getName() + " rules me");
    }
}
