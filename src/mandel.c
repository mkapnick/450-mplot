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
    uint8_t pixels[HEIGHT][WIDTH][3];
    
    double color;
    uint8_t blue;
    uint8_t green;
    uint8_t red;
    
    int i =0;
    int j =0;
    int k =0;
    
    int xMax;
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

    color = 0; 
    xMax = 1;
    height = 0;
    width = 0;
    
    x =  testArray[0];
    y = testArray[1];
    endx = testArray[2];
    endy = testArray[3];
    
    a = 0;
    b = 0;
    newa = 0;
    newb = 0;
    int count = 0;
    
    double incrementX = x + ((endx - x) / HEIGHT);
    double incrementY = y + ((endy - y) / WIDTH);
    
    for (i=0; i < HEIGHT; i++)
    {
        for (j =0; j < WIDTH; j++)
        {
            for (k =0; k < NUM_ITERATIONS && a*a+b*b < 100; k++)
            {
                count++;
                newa = a * a - b * b + x;
                newb = 2 * a * b + y;
                a=newa;
                b=newb;

                blue = 230;
                green = 50;
                red = 70;
                pixels[i][j][0] = blue;
                pixels[i][j][1] = green;
                pixels[i][j][2] = red;
                
            }
            //calculate the color
            
            if(k < 50) color = k + 50;
            else if (k < 100) color = k + 55;
            else if (k < 350) color = k + 80;
            else if (k < 500) color = k + 100;
            
            blue = ((double)color / (double)NUM_ITERATIONS * 180);
            green = ((double) color / (double)NUM_ITERATIONS * 255);
            red = ((double)color /  (double)NUM_ITERATIONS * 120);
            
            pixels[i][j][0] = blue;
            pixels[i][j][1] = green;
            pixels[i][j][2] = red;
            
            /*if(i == 780 && j == 800) //debugging statement
            {
                printf("printing random pixel\n");
                printf("%" PRIu8 "\n", green);
                printf("%" PRIu8 "\n", pixels[780][800][1] );
            }*/
            
            a=0;
            b=0;
            y += incrementY;
        }
        x += incrementX;
    }
    
    printf("Count is: %i\n",count);
    write_file(outputName, (uint8_t *) pixels);
}