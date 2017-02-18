package view;

import model.*;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

/**
 * Created by ik34-admin on 28.12.2016.
 */
public class ConsoleDisplay implements Displayable
{
    private static ConsoleDisplay instance;

    @Override
    public void displayBoard(Board board)
    {
        for(int i=0; i<board.getDimension(); i++)
        {
            for (int j=0; j<board.getDimension(); j++)
            {
                System.out.print(board.getPoint(i, j).getPointStatus() + " ");
            }
            System.out.println("");
        }
        System.out.println("");
    }

    @Override
    public void showWinner(String player)
    {
        System.out.println(player + " is the winner");
    }

    @Override
    public void showWinner()
    {
        System.out.println("Nobody is the winner");
    }

    @Override
    public void greetPlayers(String playerX, String playerO)
    {
        System.out.println("Hello, " + playerX + " and " + playerO);
    }

    @Override
    public String getPlayerName() {
        String name = "";
        System.out.println("Hi, type your name!");
        try
        {
            BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
            name = br.readLine();
        }
        catch (IOException e)
        {
            e.printStackTrace();
        }
        return name;
    }

    @Override
    public void getPlayerType() {

    }

    public static ConsoleDisplay getInstance() {
        if (instance == null)
            instance = new ConsoleDisplay();
        return instance;
    }
}
