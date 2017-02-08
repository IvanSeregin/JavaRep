package view;

import controller.GameController;
import model.*;
import model.Point;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

/**
 * Created by ik34-admin on 06.02.2017.
 */
public class GraphicsDisplay extends JFrame implements Displayable {

    public static final int SIZE = 3;
    JButton array[][] = new JButton[SIZE][SIZE];

    public GraphicsDisplay() {
        setSize(200, 200);
        setTitle("XO Game");
        setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);

        JPanel jPanel = new JPanel();
        jPanel.setLayout(new GridLayout(SIZE,SIZE));
        for (int i=0; i<SIZE; i++) {
            for (int j=0; j<SIZE; j++) {
                array[i][j] = new JButton(PointStatus.EMPTY.toString());
                int finalI = i;
                int finalJ = j;
                array[i][j].addActionListener(new ActionListener() {
                    @Override
                    public void actionPerformed(ActionEvent e) {
                        System.out.println(finalI + " " + finalJ);
                        GameController.doShoot(new Point(finalI, finalJ));
                    }
                });
                jPanel.add(array[i][j]);
            }
        }
        this.add(jPanel);

        setVisible(true);
    }

    @Override
    public void displayBoard(Game game) {
        for (int i=0; i<SIZE; i++) {
            for (int j=0; j<SIZE; j++) {
                array[i][j].setText(game.getBoard().getPoint(i, j).getPointStatus().toString());
            }
        }

    }

    @Override
    public Player readPlayerName(Player player) {
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
    public void showWinner(Player player) {

    }

    @Override
    public void showWinner() {

    }

    @Override
    public void greetPlayers(Player playerX, Player playerO) {

    }
}
