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
#include <inttypes.h>

/* You can tweak this parameter if you wish, but you should not need to. */
const int NUM_ITERATIONS = 500;
const int TEST_CASE_MAX_INDEX = 4;
char * outputName;

void calculate_pixels(double * x,double * y, double * endx, double * endy);


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
    outputName = output;
    
    if((int)testcase == 100)
    {
        //run all test cases
        
    }
    else if (testcase < TEST_CASE_MAX_INDEX)
    {
        int i;
        //initialize array
        for (i =0; i < 4; i++)
        {
            testArray[i] = testcases[testcase][i];
        }
        determineMandelBrotSet(testArray);
    }
    else
    {
        fprintf(stderr,"index into test case array is not a valid index. Exiting Program");
        exit(EXIT_FAILURE);
    }
    
    printf ("\nRun a test case and produce file %s\n", output);
}

void determineMandelBrotSet(double testArray [])
{
    
    /* declarations */ 
    double x,y,endx,endy;
    
    /* assignments */
    x =  testArray[0];
    y = testArray[1];
    endx = testArray[2];
    endy = testArray[3];
    
    /* debugging */
    printf("x is: %f\n", x);
    printf("y is: %f\n", y);
    printf("endx is: %f\n", endx);
    printf("endy is: %f\n", endy);
    
    /* Calculate mandelbrot set */
    calculate_pixels(&x,&y,&endx,&endy);
}

void calculate_pixels( double * x, double * y, double *endx, double * endy)
{
    /* declarations */
    uint8_t pixels[HEIGHT][WIDTH][3], blue,green,red;
    double incrementX, incrementY, firstX,color,a,b,newa,newb;
    int i, j,k,count;
    
    /* assignments */
    incrementX = ((*endx - *x) / WIDTH);
    incrementY = ((*endy - *y) / HEIGHT);
    firstX = *x;
    count = 0;
    
    /* Figure out color for each individual pixel in/out of mandelbrot set */
    for (i=0; i < HEIGHT; i++)
    {
        for (j =0; j < WIDTH; j++)
        {
            for (k =0; a*a+b*b < 100 && k < NUM_ITERATIONS; k++) //magnitude of the orbit point
            {
                newa = a * a - b * b + *x;
                newb = 2 * a * b + *y;
                a=newa;
                b=newb;
                count++;
            }
            
            if(a*a + b*b < 100)
            {
                blue = 230;
                green = 50;
                red = 70;
            }
            else
            {
                if(k < 50) color = k + 50;
                else if (k < 100) color = k + 55;
                else if (k < 350) color = k + 80;
                else if (k < 400) color = k + 90;
                else if (k < 500) color = k + (499 -k);
                
                blue = (color / (double)NUM_ITERATIONS * 180);
                green = ( color / (double)NUM_ITERATIONS * 255);
                red = (color /  (double)NUM_ITERATIONS * 120);
            }
            pixels[i][j][0] = blue;
            pixels[i][j][1] = green;
            pixels[i][j][2] = red;
     
            a=0;
            b=0;
            *x = *x + incrementX;
        }
        *y = *y + incrementY;
        *x = firstX;
    }
    
    //printf("count is: %i\n", count);
    
    /* Write pixels to binary file */
    write_file(outputName, (uint8_t *)pixels);
}
