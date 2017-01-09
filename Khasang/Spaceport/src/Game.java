/**
 * Created by NortT on 01.01.2017.
 */
public class Game
{
    private static Game game;
    Spaceport spaceport;
    Pilot pilot;

    private Game(){}

    public static Game getInstance()
    {
        if (game == null)
        {
            game = new Game();
            return game;
        }
        return game;
    }

    public void init(Spaceship spaceship)
    {
        spaceport = new Spaceport();
        MissionControlCentre missionControlCentre = new MissionControlCentre();
        spaceport.setMissionControlCentre(missionControlCentre);
        Lunchable dog = new Dog("Belka");
        this.spaceport.initWithLunchable(dog);
        //this.spaceport.initWithLunchable(spaceship);
    }

    public void start()
    {
        spaceport.startLunchable();
        pause(3000);
        spaceport.landLandable();
    }

    public Pilot getPilot()
    {
        return pilot;
    }

    public void setPilot(Pilot pilot)
    {
        this.pilot = pilot;
    }


    private void pause(int millis)
    {
        try {
            Thread.sleep(millis);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }
}
