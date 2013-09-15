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
    FILE *fp;
    fp= fopen(name, "wb");
    
    printf("size of array is: %li\n", sizeof(arr));
    
    struct tga_header header;
    header.id_length = 0;
    header.cmap_type = 0;
    header.img_type = 2;
    header.cmap_spec[0] = 0;
    header.cmap_spec[1] = 0;
    header.cmap_spec[2] = 0;
    header.cmap_spec[3] = 0;
    header.cmap_spec[4] = 0;
    header.img_spec[0] = 0;
    header.img_spec[1] = 0;
    header.img_spec[2] = 0;
    header.img_spec[3] = 0;
    header.img_spec[4] = 0x80;
    header.img_spec[5] = 7;
    header.img_spec[6] = 0xA0;
    header.img_spec[7] = 5;
    header.img_spec[8] = 0x18;
    header.img_spec[9] = 0;
    
    printf("HEIGHT * WIDTH IS: %i\n", HEIGHT * WIDTH);
    
    printf("size of header is: %li\n", sizeof(header));
    fwrite(&header, sizeof(uint8_t) , sizeof(header),fp);
    size_t size = fwrite(arr, sizeof(arr), HEIGHT * WIDTH, fp);
    printf("size of file is %zi\n", size);
    
    return true;
}
