/**
 * Created by NortT on 27.12.2016.
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

    public int getDimension()
    {
        return n;
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

    public Point getPoint(int x, int y)
    {
        return board[x][y];
    }

    public boolean isTheEnd()
    {
        int row = 0, cols = 0, mainDiag = 0, subDiag = 0;
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                //check rows
                if (!board[i][j].isEmpty())
                {
                    if (board[i][0].getPointStatus().equals(board[i][j].getPointStatus()))
                    {
                        row++;
                    }
                }

                //check cols
                if (!board[j][i].isEmpty())
                {
                    if (board[0][i].getPointStatus().equals(board[j][i].getPointStatus()))
                    {
                        cols++;
                    }
                }

                //check main diagonal
                if ( i == j)
                    if (!board[i][j].isEmpty())
                    {
                        if (board[0][0].getPointStatus().equals(board[i][j].getPointStatus()))
                        {
                            mainDiag++;
                        }
                    }

                //check sub diagonal
                if (!board[i][j].isEmpty() && (i+j)==(n-1))
                {
                    if (board[0][n-1].getPointStatus().equals(board[i][j].getPointStatus()))
                    {
                        subDiag++;
                    }
                }
            } //end of for_j

            if (row == n || cols == n || mainDiag == n || subDiag == n)
            {
                return true;
            } else
            {
                row = 0;
                cols = 0;
            }
        } //end of for_i

        if (mainDiag == n || subDiag == n)
        {
            return true;
        }

        return false;
    }
}
