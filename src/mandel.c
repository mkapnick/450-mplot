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
        //initialize array
        for (i =0; i < 4; i++)
        {
            testArray[i] = testcases[testcase][i];
        }
        printf("here");
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

    uint8_t pixels[HEIGHT][WIDTH][3];
    
    uint8_t color;
    uint8_t blue;
    uint8_t green;
    uint8_t red;
    
    int ok;
    int xMax;
    int j;
    int height;
    int width;
    
    double x;
    double y;
    double endx;
    double endy;
    double a;
    double b;
    double newa;
    double newb;
    
    ok = 1;
    xMax = 1;
    height = 0;
    width = 0;
    x = -0.149167; //testArray[0]
    y = 0.850833;  //testArray[1]
    endx = testArray[2];
    endy = testArray[3];
    a = 0;
    b = 0;
    newa = 0;
    newb = 0;
    
    double incrementX = x + ((endx - x) / HEIGHT);
    double incrementY = y + ((endy - y) / WIDTH);

    while (ok)
    {
        for (j =0; j < NUM_ITERATIONS; j++)
        {
            newa = a * a - b * b + x;
            newb = 2 * a * b + y;
            a=newa;
            b=newb;
            
            if((a*a) + (b*b) > 100)
            {
                printf("a^2 + b^2 is > 100\n");
                
                if (j < 50)
                    color = j + 50;
                else if (j < 100)
                    color = j + 55;
                else if (j < 350)
                    color = j + 80;
                
                blue = ((double)color / (double)NUM_ITERATIONS * 180);
                green = ((double) color / (double) NUM_ITERATIONS * 255);
                red = ((double)color / (double) NUM_ITERATIONS * 120);
                
                printf("B: %x\n", blue);
                printf("G: %x\n",green);
                printf("R: %x\n",red);
                
                pixels[height][width][0] = blue; //blue
                pixels[height][width][1] = green; //green
                pixels[height][width][2] = red; //red
            }
            else
            {                
                pixels[height][width][0] = 230; //blue
                pixels[height][width][1] = 50; //green
                pixels[height][width][2] = 70; //red
            }
        }
    
        j = 0;
        a = 0;
        b = 0;
        
        if( x <= endx && xMax)
        {
            x += incrementX;
            height ++;
        }
        else if (y <= endy)
        {
            xMax = 0;
            x = testArray[0];
            y+=incrementY;
            width++;
        }
        else
        {
            //write to the tga file
            ok = 0;
        }
    }
    printf("finished");
}

void writeToTGAFile(uint8_t *** pixels)
{
    FILE *fp;
    fp = fopen("test.bin", "wb");
    //fwrite
}