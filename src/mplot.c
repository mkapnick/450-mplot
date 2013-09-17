#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include "mplot.h"
#include "mandel.h"
#include <ctype.h>

int aflag;
int f_flag;
int pflag;
char* tgaFileName;
int testCase;
uint8_t RUN_ALL_TEST_CASES = 100;



void show_usage (void)
{
    printf ("Usage: mplot [options]\n");
    printf ("Options:\n");
    printf ("  -a            Run all test cases; this flag is incompatible with -t\n");
    printf ("                (if -f is used, the name is appended with the test case number and .tga)\n");
    printf ("  -f F.tga      Specify the .tga file to use as output\n");
    printf ("                (default name is mandelbrot.tga)\n");
    printf ("  -h            Print this message and exit\n");
    printf ("  -p P          Specify pthread option\n");
    printf ("                (P = 1 means 1 thread per row; P = 2 means 1 thread per pixel)\n");
    printf ("  -t T          Run test case number T; this flag is incompatible with -a\n");
    printf ("                (default is test case 0)\n");
}

void set_flags(int argc, char * argv[])
{
    aflag = 0;
    pflag = 0;
    tgaFileName = "mandelbrot.tga"; //default file name
    testCase = 0; //default test case
    
    int c;
    opterr = 0;
    
    while ((c = getopt (argc, argv, "a::f:h:p:t:")) != -1)
        switch (c)
    {
        case 'a':
            
            printf ("in case a\n");
            if (!strcmp(argv[optind -1], "-t")) // condition returns 0 if true, 1 if false
            {
                fprintf(stderr, "flag -t is incompatible with -a. Aborting" );
                exit(EXIT_FAILURE);
            }
            
            if(!strcmp(argv[optind -1], "-f"))
            {
                // the name is appended with the test case number and .tga ...?
            }
            else
            {
                testCase = 100; // flag for running all test cases
            }
            
            aflag = 1;
            break;
            
        case 'f':
            
            printf ("in case f\n");
            printf("argument is %s\n", argv[optind - 1]);
            tgaFileName = (argv[optind - 1]);
            printf("file name is: %s\n", tgaFileName);
            break;
            
        case 'h':
            printf("in case h\n");
            //only prints first word, will there be an actual sentence here?
            printf("%s", argv[optind -1]);
            exit(EXIT_SUCCESS);
            break;
            
        case 'p':
            
            printf("in case p\n");
            printf("%s\n", argv[optind - 1]);
            
            switch (atoi(argv[optind -1]))
        {
            case 1:
                // 1 thread per row
                pflag =  1;
                break;
            case 2:
                // 1 thread per pixel
                pflag = 2;
                break;
            default:
                fprintf(stderr,"There was an error parsing the p flag. Exiting.");
                exit(EXIT_FAILURE);
        }
            break;
            
        case 't':
            printf ("in case t\n");
            if (!strcmp(argv[optind -1], "-a")) // condition returns 0 if true, 1 if false
            {
                fprintf(stderr, "flag -a is incompatible with -t. Aborting" );
                exit(EXIT_FAILURE);
            }
            //if(isdigit(atoi(argv[optind -1])))
            //{
                testCase = atoi(argv[ optind -1]);
            //}
            //else
            //{
               // fprintf(stderr, "Must provide valid test case number. Exiting program");
                //exit(EXIT_FAILURE);
                
            //}
            break;
            
        case '?':
            if (optopt == 'f')
                fprintf (stderr, "Option -%c requires an argument.\n", optopt);
            else if(optopt == 'h')
                fprintf (stderr, "Option -%c requires an argument.\n", optopt);
            else
                fprintf (stderr,
                         "Unknown option character `\\x%x'.\n",
                         optopt);
            
        default:
            fprintf(stderr, "Flags could not be parsed");
            exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[])
{
    //show_usage ();
    set_flags(argc,argv);
    printf("testcase is: %i\n", testCase);
    run_test_case (tgaFileName, testCase, pflag);
    
    return 0;
}