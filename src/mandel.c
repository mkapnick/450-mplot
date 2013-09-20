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
char * strcat ( char * destination, const char * source );
const int NUM_ITERATIONS = 500;
const int TEST_CASE_MAX_INDEX = 7;
char * outputName;

uint8_t pixels [HEIGHT][WIDTH][3];
double x,y,endx,endy, incrementX, incrementY, firstX;

struct info {
    double x;
    double y;
    double endx;
    double endy;
    double firstX;
    double incrementX;
    double incrementY;
    long height;
    int width;
};
struct info original;



/* The main entry point into the Mandelbrot set calculation. The parameters
 * to this function consist of the name of the file to generate (output), the
 * testcase number to run (testcase), and the pthreads option (0 means do not
 * run as a multithreaded process, 1 means generate 1 thread per row, 2 means
 * generate 1 thread per pixel). The testcase parameter must correspond to a
 * valid index in the testcases array (defined in mandel.h).
 */
void
run_test_case (char * output, uint8_t testcase, uint8_t pthreads, int length_of_file)
{
    double testArray[5];
    outputName = output;
    
    if(testcase == 100)
    {
        //run all test cases
        printf("\n| ----- Run all test cases as separate processes ----- |\n\n");
        int count,j;
        count = 0;
        do
        {
            for (j =0; j < 4; j++)
            {
                testArray[j] = testcases[count][j];
            }
            count++;
            
            printf("outputname is %s\n", outputName);
            determineMandelBrotSet(testArray);
            calculate_pixels();
            printf("---------------------------------------\n");
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
            printf("\n| ----- Multithreaded program ----- | \n\n");
            determineMandelBrotSet(testArray);
            calculate_per_row();
        }
        
        else if (pthreads == 2)
        {
            printf("Multithreaded program, per_pixel. Exiting program because of segmentation fault\n");
            exit(EXIT_SUCCESS);
        }
        else
        {
            printf("\n| ----- Unithreaded program ----- | \n\n");
            determineMandelBrotSet(testArray);
            calculate_pixels();
        }
    }
    else
    {
        fprintf(stderr,"index into test case array is not a valid index. Exiting Program\n");
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
    original.x =  testArray[0];
    original.y = testArray[2];
    original.endx = testArray[1];
    original.endy = testArray[3];
    original.firstX = original.x;
    
    if ((original.x >0 && original.endx > 0) || (original.x <0 && original.endx < 0))
        original.incrementX = fabs(fabs(original.endx) - fabs(original.x) ) / (double)WIDTH;
    else
        original.incrementX = (fabs(original.endx) + fabs(original.x)) / (double)WIDTH;
    
    if ((original.y >0 && original.endy > 0) || (original.y <0 && original.endy < 0))
        original.incrementY = fabs(fabs(original.endy) - fabs(original.y)) / (double)HEIGHT;
    else
        original.incrementY = (fabs(original.endy) + fabs(original.y)) / (double)HEIGHT;
    
    /* debugging */
    printf("original.x is: %f\n", original.x);
    printf("original.y is: %f\n", original.y);
    printf("original.x is: %f\n", original.endx);
    printf("original.y is: %f\n", original.endy);
    
    /* Calculate pioriginal.xel color based on algorithm */
    
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

    printf("increment x is: %f\n", original.incrementX);
    printf("increment y is: %f\n", original.incrementY);
    
    for (i=0; i < HEIGHT; i++)
    {
        for (j =0; j < WIDTH; j++)
        {
            find_color(&cp, &original);
            pixels[i][j][0] = cp.blue;
            pixels[i][j][1] = cp.green;
            pixels[i][j][2] = cp.red;
            original.x = original.x + original.incrementX;
        }
        original.y = original.y + original.incrementY;
        original.x = original.firstX;
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
    
    struct info infos[HEIGHT];
    int m;
    
    //initialize each struct
    for (m=0; m < HEIGHT; m++)
    {
        infos[m].x = original.x;
        infos[m].y = original.y;
        infos[m].endx = original.endx;
        infos[m].endy = original.endy;
        infos[m].incrementX = original.x;
        infos[m].incrementY = original.incrementY;
    }
    
    long i,t;
    void * status;
    
    pthread_t threads[HEIGHT];
    pthread_attr_t attr;
    int rc;
    
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    
    for (i=0; i < HEIGHT; i++)
    {
        rc = pthread_create(&threads[i], &attr, width_and_color, (void *) &infos[i]);
        infos[i].height ++;
    }
    
    pthread_attr_destroy(&attr);
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
    pthread_exit(NULL);
}

/**************************************
 *
 **************************************/

void * width_and_color(void * ii)
{
    struct info * reference= (struct info *)ii;
    
    int j;
    struct colorpixels cp;
    
    for (j =0; j < WIDTH; j++)
    {
        find_color(&cp, &reference);
        pixels[reference->height][j][0] = cp.blue;
        pixels[reference->height][j][1] = cp.green;
        pixels[reference->height][j][2] = cp.red;
        reference->x += reference ->incrementX;
    }
    
    reference->y += reference->incrementY;
    reference->x = reference->firstX;
    return 0;
}

void * find_color(void * cp2, void * rf)
{
    struct colorpixels *cp;
    struct info * r;
    cp = (struct colorpixels *) cp2;
    r = (struct info *) rf;
    
    int k;
    uint8_t blue,green,red;
    double a,b,newa,newb, color;
    
    a=0;
    b=0;
    color = 0;
    
    for (k =0; a*a+b*b < 100 && k < NUM_ITERATIONS; k++) //magnitude of the orbit point
    {
        newa = a * a - b * b + r->x;
        newb = 2 * a * b + r->y;
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
    
    return 0;
}


