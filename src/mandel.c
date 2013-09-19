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

uint8_t pixels [HEIGHT][WIDTH][3];
double x,y,endx,endy, incrementX, incrementY, firstX;


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
    
    if(testcase == 100)
    {
        //run all test cases
        //while loop and for loop
        int count,j;
        count = 0;
        do
        {
            for (j =0; j < 4; j++)
            {
                testArray[j] = testcases[count][j];
            }
            count++;
            determineMandelBrotSet(testArray);
        } while (count < 4);
        
    }
    else if (testcase < TEST_CASE_MAX_INDEX)
    {
        int i;
        //initialize array
        for (i =0; i < 4; i++)
        {
            testArray[i] = testcases[testcase][i];
        }
        
        /* initialized test array, now determine mandelbrot set */
        if (pthreads == 1)
        {
            printf("here");
            determineMandelBrotSet(testArray);
            calculate_per_row();
        }
        
        else if (pthreads == 2)
        {
            // can't do
        }
        else
        {
            determineMandelBrotSet(testArray);
            calculate_pixels();
        }
    }
    else
    {
        fprintf(stderr,"index into test case array is not a valid index. Exiting Program");
        exit(EXIT_FAILURE);
    }
    
    printf ("\nFinished\n");
}

/**************************************
 *
 **************************************/

void * determineMandelBrotSet(double testArray [])
{
    /* assignments */
    x =  testArray[0];
    y = testArray[2];
    endx = testArray[1];
    endy = testArray[3];
    firstX = x;
    
    if ((x >0 && endx > 0) || (x <0 && endx < 0))
        incrementX = fabs(fabs(endx) - fabs(x) ) / (double)WIDTH;
    else
        incrementX = (fabs(endx) + fabs(x)) / (double)WIDTH;
    
    if ((y >0 && endy > 0) || (y <0 && endy < 0))
        incrementY = fabs(fabs(endy) - fabs(y)) / (double)HEIGHT;
    else
        incrementY = (fabs(endy) + fabs(y)) / (double)HEIGHT;
    
    /* debugging */
    printf("x is: %f\n", x);
    printf("y is: %f\n", y);
    printf("endx is: %f\n", endx);
    printf("endy is: %f\n", endy);
    
    /* Calculate pixel color based on algorithm */
    
    return 0;
    
}

/**************************************
 *
 **************************************/

void calculate_pixels()
{
    /* declarations */
    int i,j;
    struct colorpixels cp;
    
    /* assignments */

    
    printf("increment x is: %f\n", incrementX);
    printf("increment y is: %f\n", incrementY);
    
    for (i=0; i < HEIGHT; i++)
    {
        for (j =0; j < WIDTH; j++)
        {
            find_color(&cp);
            pixels[i][j][0] = cp.blue;
            pixels[i][j][1] = cp.green;
            pixels[i][j][2] = cp.red;
            x = x + incrementX;
        }
        y = y + incrementY;
        x = firstX;
    }
    /* Write pixels to binary file */
    write_file(outputName, (uint8_t *)pixels);
}


/**************************************
 *
 **************************************/

void calculate_per_row()
{
    /* declarations */
    long i;
    void * status;
    
    /* assignments */
    
    pthread_t threads[HEIGHT];
    pthread_attr_t attr;
    int rc;
    
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    
    for (i=0; i < HEIGHT; i++)
    {
        rc = pthread_create(&threads[i], &attr, width_and_color, &i);
    }
    pthread_attr_destroy(&attr);
    long t;
    for(t=0; t<HEIGHT; t++)
    {
        rc = pthread_join(threads[t], &status);
        if (rc)
        {
            printf("ERROR; return code from pthread_join() is %d\n", rc);
            exit(-1);
        }
        printf("Main: completed join with thread %ld having a status of %ld\n",t,(long)status);
    }
    write_file(outputName, (uint8_t *)pixels);
}

/**************************************
 *
 **************************************/

void * width_and_color(void * ii)
{
    int i = (int) (long)ii;

    struct colorpixels cp;
    int j;
    
    for (j =0; j < WIDTH; j++)
    {
        find_color(&cp);
        pixels[i][j][0] = cp.blue;
        pixels[i][j][1] = cp.green;
        pixels[i][j][2] = cp.red;
        x = x + incrementX;
    }
    y = y + incrementY;
    x = firstX;
    
    return 0;
}

void * find_color(void * cp2)
{
    struct colorpixels *cp;
    cp = (struct colorpixels *) cp2;
    int k;
    uint8_t blue,green,red;
    double a,b,newa,newb, color;
    
    a=0;
    b=0;
    color = 0;
    
    for (k =0; a*a+b*b < 100 && k < NUM_ITERATIONS; k++) //magnitude of the orbit point
    {
        newa = a * a - b * b + x;
        newb = 2 * a * b + y;
        a=newa;
        b=newb;
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
        else color = k + (499 -k);
        
        blue = (color / (double)NUM_ITERATIONS * 180);
        green = ( color / (double)NUM_ITERATIONS * 255);
        red = (color /  (double)NUM_ITERATIONS * 120);
    }
    
    cp->blue = blue;
    cp->green = green;
    cp->red = red;
    
    //printf("blue is: %f\n", (double)blue);
   // printf("green is: %f\n", green);
   // printf("red is: %f\n", red);
    return 0;
}


