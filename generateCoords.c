#include"headers.h"

//
// Code to generate random ground
//

// Grid len, size of one side of two dimensional array.
void groundGeneration(double coords[][100], int gridLen)
{
    int upDown;
    int rDegree;
    double ave;
    // Make the first row on it's own.
    coords[0][0] = 0;
    for (int k = 1; k < (gridLen-1); k++)
    {
        upDown = rand() % 5;
        // Rand int to decide by how much
        rDegree = rand() % 10;
        ave = coords[0][k-1];

        if (upDown == 0)
            {
                if (rDegree <= 4)
                {
                    coords[0][k] = ave + .05;
                }
                else if (rDegree <= 8)
                {
                    coords[0][k] =  ave + .15;
                }

                else 
                {
                    coords[0][k] = ave + .30;
                }
            }
            // If 1, then terrain goes down
            if (upDown == 1)
            {
                if (rDegree <= 5)
                {
                    coords[0][k] = ave - .05;
                }
                else if (rDegree <= 8)
                {
                    coords[0][k] =  ave - .15;
                }

                else 
                {
                    coords[0][k] = ave - .30;
                }
            }
            else
            {
                coords[0][k] = ave;
            }

    }

    for (int i = 1; i < (gridLen-1); i++)
    {
        for (int j = 0; j < gridLen; j++)
        {
            // Rand int to decide if up or down
            int upDown = rand() % 5;
            // Rand int to decide by how much
            int rDegree = rand() % 100;
            if (j == 0)
            {
                ave = coords[i-1][j];
            }
            else 
            {
                ave = (coords[i-1][j] + coords[i][j-1]) / 2;
            }
            
            // If 0, then terrain goes up
            if (upDown == 0)
            {
                if (rDegree <= 40)
                {
                    coords[i][j] = ave + .05;
                }
                else if (rDegree <= 80)
                {
                    coords[i][j] =  ave + .15;
                }

                else if (rDegree <= 99)
                {
                    coords[i][j] = ave + .30;
                }
                else 
                {
                    coords[i][j] = ave + .5;
                }
            }
            // If 1, then terrain goes down
            else if (upDown == 1)
            {
                if (rDegree <= 40)
                {
                    coords[i][j] = ave - .05;
                }
                else if (rDegree <= 80)
                {
                    coords[i][j] =  ave - .15;
                }

                else if (rDegree <= 99)
                {
                    coords[i][j] = ave - .30;
                }
                else 
                {
                    coords[i][j] = ave - .5;
                }
            }
            // Terrain stays the same
            else
            {
                coords[i][j] = ave;
            }
            
        }
    }
}

void treeGeneration(double trees[][100][3])
{
    for(int i = 1; i < 99; i++)
    {
        for(int k = 1; k < 99; k++)
        {
            int rNum = rand() % 20;
            if(rNum <= 17)
            {
                trees[i][k][0] = 0;
            }
            else
            {
                trees[i][k][0] = 1;

                double rint = (rand() % 4) + 3;
                trees[i][k][1] = (rint / 100.0) * 20.0;
                trees[i][k][2] = rand() % 60;          
            }
        }
    }
}