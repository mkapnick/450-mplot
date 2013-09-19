#ifndef __RUN_MANDEL__
#define __RUN_MANDEL__

struct colorpixels {
    uint8_t blue;
    uint8_t green;
    uint8_t red;
};

void run_test_case (char * output, uint8_t testcase, uint8_t pthreads);
void *determineMandelBrotSet(double []);
void calculate_pixels();
void calculate_per_row();
void * find_color(void * cp2);
void * width_and_color(void * ii);

#endif