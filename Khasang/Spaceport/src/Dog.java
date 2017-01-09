/**
 * Created by NortT on 01.01.2017.
 */
public class Dog implements Lunchable, Landable
{
    String name;

    public Dog(String name)
    {
        this.name = name;
    }

    @Override
    public void lunch()
    {
        System.out.println(name + " is in space!");
    }

    @Override
    public void landing()
    {
        System.out.println("My name is " + name + " and I've landed right now");
    }
}
