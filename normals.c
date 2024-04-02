#include"headers.h"

/*
##########################
Function to calculate smooth normals
###########################
*/

// Started with code from example 13. Built on it from there.
void groundNormals(double ground[][100], int gridLen, double normals[][100][3])
{
    for(int i = 1; i < 100; i++)
    {
        for(int k = 1; k < 100; k++)
        {
            float dx0 = (i)-(i);
            float dy0 = ground[i][k]-ground[i+1][k];
            float dz0 = (k)-((k+1));

            float dx1 = ((i+1))-(i);
            float dy1 = ground[i+1][k]-ground[i][k];
            float dz1 = (k)-(k);
            //  Normals
            float Nx = dy0*dz1 - dy1*dz0;
            float Ny = dz0*dx1 - dz1*dx0;
            float Nz = dx0*dy1 - dx1*dy0;
            
            // Changed to a negatives since it was pointing the other way and this was the fastest solution.
            normals[i][k][0] += -Nx;
            normals[i][k][1] += -Ny;
            normals[i][k][2] += -Nz;

            float ndx0 = (i)-((i+1));
            float ndy0 = ground[i][k+1]-ground[i+1][k+1];
            float ndz0 = ((k+1))-((k+1));

            float ndx1 = ((i+1))-(i);
            float ndy1 = ground[i+1][k]-ground[i][k+1];
            float ndz1 = (k)-((k+1));

            //  Normals
            float nNx = ndy0*ndz1 - ndy1*ndz0;
            float nNy = ndz0*ndx1 - ndz1*ndx0;
            float nNz = ndx0*ndy1 - ndx1*ndy0;

            normals[i][k+1][0] += -nNx;
            normals[i][k+1][1] += -nNy;
            normals[i][k+1][2] += -nNz;

            normals[i+1][k][0] += -Nx;
            normals[i+1][k][1] += -Ny;
            normals[i+1][k][2] += -Nz;

            normals[i+1][k+1][0] += -nNx;
            normals[i+1][k+1][1] += -nNy;
            normals[i+1][k+1][2] += -nNz;

            
        }
    }

    for (int i = 0; i < 100; i++) 
    {
        for (int k = 0; k < 100; k++) 
        {
            // Got the solution for how to normalize from https://stackoverflow.com/questions/10002918/what-is-the-need-for-normalizing-a-vector
            float length = sqrt(normals[i][k][0] * normals[i][k][0] + normals[i][k][1] * normals[i][k][1] + normals[i][k][2] * normals[i][k][2]);
            if (length != 0) 
            {
                normals[i][k][0] /= length;
                normals[i][k][1] /= length;
                normals[i][k][2] /= length;
            }
        }
    }
}