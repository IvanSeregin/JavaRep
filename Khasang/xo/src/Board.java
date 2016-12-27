/**
 * Created by ik34-admin on 27.12.2016.
 */
public class Board
{
    final int n=3;
    Point board[][];

    public Board()
    {
        board = new Point[n][n];

        PointFactory pointFactory = new PointFactory();
        for(int i=0; i<n; i++)
            for (int j=0; j<n; j++)
            {
                board[i][j] = pointFactory.createPoint(i, j, new PointStatus("_"));
            }
    }

    public boolean isFull()
    {
        for(int i=0; i<n; i++)
            for (int j=0; j<n; j++)
                if (board[i][j].isEmpty())
                    return false;
        return true;
    }

    public boolean setPoint(Point point)
    {
        if ((board[point.getX()][point.getY()].getPointStatus() == "_"))
        {
            board[point.getX()][point.getY()] = point;
            return true;
        }
        return false;
    }

    public void show()
    {
        for(int i=0; i<n; i++)
        {
            for (int j=0; j<n; j++)
            {
                System.out.print(board[i][j].getPointStatus() + " ");
            }
            System.out.println("");
        }
        System.out.println("");

    }
}
