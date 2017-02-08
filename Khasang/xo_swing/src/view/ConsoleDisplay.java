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
    @Override
    public void displayBoard(Game game)
    {
        for(int i=0; i<game.getBoard().getDimension(); i++)
        {
            for (int j=0; j<game.getBoard().getDimension(); j++)
            {
                System.out.print(game.getBoard().getPoint(i, j).getPointStatus() + " ");
            }
            System.out.println("");
        }
        System.out.println("");
    }

    @Override
    public Player readPlayerName(Player player)
    {
        try
        {
            BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
            System.out.println("Hello, "+ player.getSign() + " player! Write yur name, pls: ");
            String name = br.readLine();
            player.setName(name);
        }
        catch (IOException e)
        {
            e.printStackTrace();
        }
        return player;
    }

    @Override
    public void showWinner(Player player)
    {
        System.out.println(player + " is the winner");
    }

    @Override
    public void showWinner()
    {
        System.out.println("Nobody is the winner");
    }

    @Override
    public void greetPlayers(Player playerX, Player playerO)
    {
        System.out.println("Hello, " + playerX + " and " + playerO);
    }
}
