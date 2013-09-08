#include <stdio.h>
#include <fcntl.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>
#include <stdint.h>
#include "mandel.h"
#include "tga.h"
#define __INCLUDE_MANDEL_ARRAY__
#include "mplot.h"

/* You can tweak this parameter if you wish, but you should not need to. */
const int NUM_ITERATIONS = 500;
const int TEST_CASE_MAX_INDEX = 4;

struct rec {
    
    
};

/* The main entry point into the Mandelbrot set calculation. The parameters
 * to this function consist of the name of the file to generate (output), the
 * testcase number to run (testcase), and the pthreads option (0 means do not
 * run as a multithreaded process, 1 means generate 1 thread per row, 2 means
 * generate 1 thread per pixel). The testcase parameter must correspond to a
 * valid index in the testcases array (defined in mandel.h).
 */
void
run_test_case (char * output, uint8_t testcase, uint8_t pthreads)
{
    double testArray[4];
    
    if(testcase == -1)
    {
        //run all test cases
    }
    else if (testcase < TEST_CASE_MAX_INDEX && testcase >=0)
    {
        int i;
        int j;
        double x;
        double y;
        double a;
        double b;
        double z;
        double newa;
        double newb;
        double newz;
        
        FILE *fp;
        struct rec r;
        uint8_t pixels[HEIGHT][WIDTH][3];
        
        
        //initialize array
        for (i =0; i < 4; i++)
        {
            testArray[i] = testcases[testcase][i];
        }
        
        //initialize variables
        x = testArray[0];
        y = testArray[2];
        a = 0;
        b = 0;
        newa = 0;
        newb = 0;
        newz = 0;
        z = a+b;
        
        fp=fopen(output, "rb");
        
        // apply algorithm 500 times
        for (j =0; j < NUM_ITERATIONS; j++)
        {
            newa = a * a - b * b + x;
            newb = 2 * a * b + y;
            a=newa;
            b=newb;
            newz=a+b;
            z=newz;
            printf("a is: %f\n", a);
            printf("b is: %f\n", b);
            printf("z is: %f\n", z);
            printf("---------------------------\n");
            
            if((a*a) + (b*b) > 100)
            {
                //z is not in the set
                printf("a^2 + b^2 is > 100!!!\n");
            }
            else
            {
               // RGB values should be red=70,green=50,blue=230
                // REMEMBER, the pattern goes ordered as blue,green,red in tga file
                fread(&r, 17,1,fp);
                //pixels[][][0] = 230;
                //pixels[][][1] = 50;
                //pixels[][][2] = 70;
            }
        }
    }
    else
    {
        fprintf(stderr,"index into test case array is not a valid index. Exiting Program");
        exit(EXIT_FAILURE);
    }

    printf ("Run a test case and produce file %s\n", output);
}

/* Algorithm for generating the color for a single pixel:
 *
 *   Consider (x,y) in the Argand plane, corresponding to x + yi
 *
 *   Start with z = a + bi, where a = b = 0 (so z = 0)
 *
 *   Repeat the following calculation NUM_ITERATIONS times:
 *
 *     newz = z^2 + (x + yi)
 *          = (a + bi) * (a + bi) + (x + yi)
 *          = (a^2 - b^2 + x) + (2ab + y)i
 *
 *   SUGGESTION: Use two double variables to represent the two components of a
 *   complex number. So the above calculation could be done in code as:
 *
 *     newa = a * a - b * b + x;
 *     newb = 2 * a * b + y;
 *     a = newa;
 *     b = newb;
 *
 *   If the magnitude of the "orbit point" ever exceeds 100, then we know the
 *   point is not in the set. For the number z = a + bi, we calculate this as
 *   follows:
 *
 *     if (a^2 + b^2 > 100) then z is not in the set
 *
 *   For more help, see the pseudocode in the Wikipedia article.
 *
 *   If the point IS in the set, the RGB values should be red=70, green=50,
 *   blue=230. NOTE: Each of these corresponds to one byte in the TGA file,
 *   which expects the bytes to be ordered as blue, then green, then red.
 *   If your image does not look right, you can use hexdump to examine the
 *   values and make sure that the values are correct. Within the hex dump,
 *   you should see repeated patterns of:
 *
 *     E6 32 46
 *
 *   For points NOT in the set, we want the color to be relative to the number
 *   of iterations before the threshold for the orbit point magnitude was
 *   exceeded. That is, the more iterations a point completes implies it is
 *   somehow closer to being in the set. We indicate this by making that point
 *   in the image a lighter color. The formula for calculating the color is
 *   as follows:
 *
 *     if (iterations < 50) then color = iterations + 50
 *     else if (iterations < 100) then color = iterations + 55
 *     else if (iterations < 350) then color = iterations + 80
 *
 *     blue = color / NUM_ITERATIONS * 180
 *     green = color / NUM_ITERATIONS * 255
 *     red = color / NUM_ITERATIONS * 120
 *
 *  Note that "color" can take on any number up to NUM_ITERATIONS, which is
 *  500 by default. On the other hand, blue, green, and red, will only take
 *  on values 0 - 255. Since you're going to have to write those out to a file
 *  that expects each one to take up only 1 byte, uint8_t would be a good type
 *  to choose.
 *
 *  HINT: It would be wise to write a single function to do the pixel
 *  calculation. You can then use that function for either the non-threaded
 *  or multithreaded version. Here are some sample arguments and results:
 *
 *    x = -0.149167, y = 0.850833
 *      has RGB 18, 38, 27
 *
 *    x = -0.135000, y = 0.870833
 *      has RGB 30, 64, 45
 *
 *    x = -0.106458, y = 0.883333
 *      has RGB 92, 192, 138
 *
 *    x = -0.1083333 y = 0.891667
 *      has RGB 70, 50, 230
 *
 */