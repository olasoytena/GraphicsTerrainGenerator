#include"headers.h"

bool collisionCilinder(double coords[][100][3], double x1, double z1, double tempx, double tempz)
{
    for (int i = 0; i < 100; i++)
    {  
        for(int j = 0; j < 100; j++)
        {
            if (coords[i][j][0] == 1)
            {
                double x2 = i;
                double z2 = j;
                double dist = coords[i][j][1];
                double dist1_2 = sqrt(pow(x2-x1,2)+pow(z2-z1,2));
                double distNew_Old = sqrt(pow(x2-tempx,2)+pow(z2-tempz,2));

                // Check if player is within object radius, and if new movement direction goes further into object.
                if (dist1_2 <= dist && dist1_2 >= distNew_Old)
                {
                    return true;
                }
            }
        }
    } 
    return false;
}