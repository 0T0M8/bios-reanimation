
#include <stdio.h>
#include <string.h> 
#include <math.h> 
#include <unistd.h>

#define WIDTH  80 
#define HEIGHT 24 
#define SIZE   (WIDTH * HEIGHT)
#define PI 3.14159265358979323846
#define R1 1.0 
#define R2 2.0 
#define K2 5.0

static const char luminance[] = ".,-~:;=!*#$@";
int main(void) { 
double zbuffer[SIZE]; 
char output[SIZE];
double A = 0.0;
double B = 0.0;

printf("\033[2J");

while (1)
{
    memset(output, ' ', sizeof(output));
    memset(zbuffer, 0, sizeof(zbuffer));

    for (double theta = 0.0; theta < 2.0 * PI; theta += 0.07)
    {
        for (double phi = 0.0; phi < 2.0 * PI; phi += 0.02)
        {
            double sin_theta = sin(theta);
            double cos_theta = cos(theta);
            double sin_phi = sin(phi);
            double cos_phi = cos(phi);

            double sin_A = sin(A);
            double cos_A = cos(A);
            double sin_B = sin(B);
            double cos_B = cos(B);

            /* Position on the donut's surface. */
            double circle = R2 + R1 * cos_theta;

            double x3d = circle * cos_phi;
            double y3d = circle * sin_phi;
            double z3d = R1 * sin_theta;

            /* Rotate around the X axis. */
            double y1 = y3d * cos_A - z3d * sin_A;
            double z1 = y3d * sin_A + z3d * cos_A;

            /* Rotate around the Z axis. */
            double x2 = x3d * cos_B - y1 * sin_B;
            double y2 = x3d * sin_B + y1 * cos_B;

            /* Perspective projection. */
            double depth = z1 + K2;
            double inverse_depth = 1.0 / depth;

            int x = (int)(WIDTH / 2.0
                + 30.0 * inverse_depth * x2);

            int y = (int)(HEIGHT / 2.0
                - 15.0 * inverse_depth * y2);

            if (x < 0 || x >= WIDTH ||
                y < 0 || y >= HEIGHT)
            {
                continue;
            }

            int index = x + WIDTH * y;

            /*
             * Approximate surface brightness using
             * the surface normal and a light direction.
             */
            double light =
                cos_theta * cos_phi * sin_B
                + cos_theta * sin_phi * sin_A * cos_B
                + sin_theta * cos_A * cos_B;

            int shade = (int)((light + 1.0) * 5.5);

            if (shade < 0)
                shade = 0;

            if (shade > 11)
                shade = 11;

            if (inverse_depth > zbuffer[index])
            {
                zbuffer[index] = inverse_depth;
                output[index] = luminance[shade];
            }
        }
    }

    /* Return the cursor to the top-left corner. */
    printf("\033[2J\033[H");

    for (int i = 0; i < SIZE; i++)
    {
        putchar(output[i]);

        if ((i + 1) % WIDTH == 0)
            putchar('\n');
    }

    fflush(stdout);

    A += 0.04;
    B += 0.02;

    usleep(30000);
}

return 0;
}
