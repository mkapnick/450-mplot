#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <stdint.h>
#include <sys/stat.h>
#include "tga.h"
#include "mplot.h"
#include <inttypes.h>

/* Opens a file and writes the contents of the pic array (representing the
 * pixels of the image) to it. Consult tga.h for a useful data structure for
 * generating the TGA header. WARNING: Depending on the compiler you are using,
 * the fields of that structure MAY be compiled out of order. If your image
 * cannot be viewed once it is generated, you may want to use the hexdump
 * utility to make sure it is being written properly.
 */
bool
write_file (char * name, uint8_t * pic)
{
    printf("in write_file. Name of file is %s\n", name);
    uint8_t (*arr)[WIDTH][HEIGHT] = (uint8_t (*)[WIDTH][HEIGHT])pic;
    // the array is: arr[COLOR][WIDTH][HEIGHT]
    //printf ("ptr[1][2][3] = %" PRIu8 "\n", arr[1][1000][1010]);
    
    /*FILE *fp;
    fp= fopen(name, "wb");
    char x[10]="ABCDEFGHIJ";
    printf("okokoko\n");
    printf("x is %" PRIu8 "\n", x[7]);
    fwrite(x, sizeof(x[0]), sizeof(x)/sizeof(x[0]), fp);*/
    
    //header info
    //00  00  02  00  00  00  00  00  00  00  00  00  07  80  05  A0
    //18  00
    
    
    
    FILE *fp;
    fp= fopen(name, "wb");
    uint8_t pixel= 0;
    int i,j,k;
    int count = 0;

    
    for (i =0; i < HEIGHT; i++)
    {
        for(j=0; j < WIDTH; j++)
        {
            count++;
            for (k=0; k < 3; k++)
            {
                pixel = arr[k][j][i];
                fwrite(arr, sizeof(pixel), sizeof(arr), fp);
            }
        }
    }
    //fwrite (, sizeof(uint8_t), , fp);
    printf("Count is: %i\n",count);
    return true;
}
