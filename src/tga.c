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
    uint32_t t;
    //struct tga_header header;
    /*printf ("pic[1][2][3] = %d\n",
            pic + (1*WIDTH*HEIGHT) + 2*HEIGHT + 3);*/
    
    uint8_t (*arr)[WIDTH][HEIGHT] =
    (uint8_t (*)[WIDTH][HEIGHT])pic;
    t=arr[700][700][0] & 0xff;
    printf ("ptr[1][2][3] = %" PRIu8"\n", arr[700][700][0] );
    
    FILE *fp;
    fp= fopen(name, "wb");
    printf("finished");
    //fwrite (, sizeof(uint8_t), , fp);
    return true;
}
