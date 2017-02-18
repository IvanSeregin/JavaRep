package view;

import controller.GameController;
import model.*;
import model.Point;

import javax.swing.*;
import java.awt.*;

import static java.lang.System.exit;

/**
 * Created by ik34-admin on 06.02.2017.
 */
public class GraphicsDisplay extends JFrame implements Displayable, Runnable {

    public static final int SIZE = 3;
    private JButton array[][] = new JButton[SIZE][SIZE];
    private GameController gameController = GameController.getInstance();
    private static GraphicsDisplay instance;

    private GraphicsDisplay() {
        setSize(200, 200);
        setTitle("XO Game");
        setLocationRelativeTo(null);
        setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);
        crateMenu();
        JPanel jPanel = new JPanel();
        jPanel.setLayout(new GridLayout(SIZE,SIZE));
        for (int i=0; i<SIZE; i++) {
            for (int j=0; j<SIZE; j++) {
                array[i][j] = new JButton(PointStatus.EMPTY.toString());
                int finalI = i;
                int finalJ = j;
                array[i][j].addActionListener(e -> gameController.doShoot(new Point(finalI, finalJ)));
                jPanel.add(array[i][j]);
            }
        }
        this.add(jPanel);

        setVisible(true);
    }

    public static GraphicsDisplay getInstance() {
        if (instance == null)
            instance = new GraphicsDisplay();
        return instance;
    }

    private void crateMenu() {
        JMenuBar menuBar = new JMenuBar();
        JMenu menu = new JMenu("File");

        JMenuItem newGameMenu = new JMenuItem("New game");
        newGameMenu.addActionListener(e-> gameController.newGame());
        //newGameMenu.addActionListener(e-> getPlayerType());

        JMenuItem exitMenu = new JMenuItem("Exit");
        exitMenu.addActionListener(e -> exit(0));

        menuBar.add(menu);

        menu.add(newGameMenu);
        menu.add(exitMenu);

        setJMenuBar(menuBar);
    }

    @Override
    public void displayBoard(Board board) {
        for (int i=0; i<SIZE; i++) {
            for (int j=0; j<SIZE; j++) {
                array[i][j].setText(board.getPoint(i, j).getPointStatus().toString());
            }
        }
    }

    @Override
    public String getPlayerName() {
        String name = JOptionPane.showInputDialog(
                this,
                "Type your name");
        return name;
    }

    @Override
    public void getPlayerType() {
        Object[] possibilities = {GameController.PlayerType.PC, GameController.PlayerType.USER};
        GameController.PlayerType playerType = (GameController.PlayerType) JOptionPane.showInputDialog(
                this, "Choose your side", "Hi player #" + gameController.getPlayerCount(),
                JOptionPane.PLAIN_MESSAGE, null, possibilities, possibilities[0]
        );
        gameController.setPlayerType(playerType);
    }

    @Override
    public void showWinner(String player) {
        JOptionPane.showMessageDialog(this, player + ", you're the winner");
    }

    @Override
    public void showWinner() {
        JOptionPane.showMessageDialog(this, "Nobody is the winner");
    }

    @Override
    public void greetPlayers(String playerX, String playerO) {
        JOptionPane.showMessageDialog(this, "Hello " + playerX + " and " + playerO);
    }


    @Override
    public void run() {

    }
}
