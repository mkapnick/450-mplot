#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include "mplot.h"
#include "mandel.h"

int aflag;
int f_flag;
int pflag;
char* tgaFileName;
int testCase;
const int RUN_ALL_TEST_CASES = -1;

void test(uint8_t * pixels);

void
show_usage (void)
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
    // * PARAMS *
    // output is the name of the file to generate
    // testcase --> testcase NUMBER to run
    // pthread option, can either be 0,1,or 2
    double testcases[][4] = {
        { -2.00,  0.80, -1.05,  1.05 }, // test case 0
        { -1.50, -0.30, -0.20,  0.70 }, // test case 1
        { -0.15, -0.07,  0.85,  0.91 }, // test case 2
        { -0.03,  0.01,  0.635, 0.665 } // test case 3
    };
    
    aflag = 0;
    pflag = 0;
    tgaFileName = "out.tga"; //default file name
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
                // run all test cases
                /*for (test = 0; test < 4; test++)
                 {
                 printf(" in loop -- %.2f\nt", testcases[0][test]);
                 }*/
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
            testCase = atoi(argv[ optind -1]);
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

void test(uint8_t * pixels)
{
    
}

int
main(int argc, char *argv[])
{
    //show_usage ();
    set_flags(argc,argv);
    
    if(aflag)
        run_test_case (tgaFileName, (uint8_t)RUN_ALL_TEST_CASES, (uint8_t)pflag);
    else
        run_test_case (tgaFileName, (uint8_t)testCase, (uint8_t)pflag);
    return 0;
}
//testing reading from binary file
/*FILE *fp;
 long lSize;
 size_t result;
 char * output = "result.tga";
 char * buffer;
 char * file = "test2.txt";
 uint8_t pixels[HEIGHT][WIDTH][3];
 
 fp=fopen(file, "r");
 
 system("pwd");
 if (fp==NULL)
 {
 fputs ("File error",stderr);
 exit (1);
 }
 
 //size of file
 fseek (fp,0,SEEK_END);
 lSize = ftell(fp);
 rewind(fp);
 
 // allocate memory to contain the whole file:
 buffer = (char *) malloc (sizeof (char *)*lSize);
 if (buffer == NULL)
 {
 fputs ("Memory error",stderr);
 exit (2);
 }
 result = fread(buffer,1,lSize,fp);
 if (result != lSize)
 {
 fputs ("Reading error",stderr);
 exit (3);
 }*/