#ifndef __RUN_MANDEL__
#define __RUN_MANDEL__

void run_test_case (char * output, uint8_t testcase, uint8_t pthreads);
void determineMandelBrotSet(double []);
void writeToTGAFile(uint8_t *** pixels);
#endif