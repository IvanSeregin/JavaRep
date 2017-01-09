/**
 * Created by NortT on 01.01.2017.
 */
public class Main
{
    public static void main(String[] args)
    {
        Game game = Game.getInstance();

        SpaceshipFactory spaceshipFactory = new SpaceshipFactory();
        Spaceship spaceship = spaceshipFactory.getNewSpaceship();

        Pilot pilot = new Pilot("James Bond 007");

        spaceship.setPilot(pilot);

        game.init(spaceship);
        game.start();
    }
}
