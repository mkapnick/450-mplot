#ifndef __MANDELBROT_TEST_CASES__
#define __MANDELBROT_TEST_CASES__

#ifdef __INCLUDE_MANDEL_ARRAY__
double testcases[][4] = {
    { -2.00,  0.80, -1.05,  1.05 }, // test case 0
    { -1.50, -0.30, -0.20,  0.70 }, // test case 1
    { -0.15, -0.07,  0.85,  0.91 }, // test case 2
    { -0.03,  0.01,  0.635, 0.665 } // test case 3
};


#endif

#ifndef __SET_FLAGS__
#define __SET_FLAGS__

void set_flags(int argc, char * argv[]);

#endif

#define MAX_TEST_CASE 3
#define WIDTH 1920
#define HEIGHT 1440

#endif

/* Algorithm for generating the color for a single pixel:
 *
 *   Consider (x,y) in the Argand plane, corresponding to x + yi
 *
 *   Start with z = a + bi, where a = b = 0 (so z = 0)
 *
 *   Repeat the following calculation NUM_ITERATIONS times:
 *
 *     newz = z^2 + (x + yi)
 *          = (a + bi) * (a + bi) + (x + yi)
 *          = (a^2 - b^2 + x) + (2ab + y)i
 *
 *   SUGGESTION: Use two double     t the two components of a complex number. So the above calculation could be done in code as:
 *
 *     newa = a * a - b * b + x;
 *     newb = 2 * a * b + y;
 *     a = newa;
 *     b = newb;
 *
 *   If the magnitude of the "orbit point" ever exceeds 100, then we know the point is not in the set. For the number z = a + bi, we calculate this as follows:
 *
 *     if (a^2 + b^2 > 100) then z is not in the set
 *
 *   For more help, see the pseudocode in the Wikipedia article.
 *
 *   If the point IS in the set, the RGB values should be red=70, green=50,
 *   blue=230. NOTE: Each of these corresponds to one byte in the TGA file,
 *   which expects the bytes to be ordered as blue, then green, then red.
 *   If your image does not look right, you can use hexdump to examine the
 *   values and make sure that the values are correct. Within the hex dump,
 *   you should see repeated patterns of:
 *
 *     E6 32 46
 *
 *   For points NOT in the set, we want the color to be relative to the number
 *   of iterations before the threshold for the orbit point magnitude was
 *   exceeded. That is, the more iterations a point completes implies it is
 *   somehow closer to being in the set. We indicate this by making that point
 *   in the image a lighter color. The formula for calculating the color is
 *   as follows:
 *
 *     if (iterations < 50) then color = iterations + 50
 *     else if (iterations < 100) then color = iterations + 55
 *     else if (iterations < 350) then color = iterations + 80
 *
 *     blue = color / NUM_ITERATIONS * 180
 *     green = color / NUM_ITERATIONS * 255
 *     red = color / NUM_ITERATIONS * 120
 *
 *  Note that "color" can take on any number up to NUM_ITERATIONS, which is
 *  500 by default. On the other hand, blue, green, and red, will only take
 *  on values 0 - 255. Since you're going to have to write those out to a file that expects each one to take up only 1 byte, uint8_t would be a good type
 *  to choose.
 *
 *  HINT: It would be wise to write a single function to do the pixel
 *  calculation. You can then use that function for either the non-threaded
 *  or multithreaded version. Here are some sample arguments and results:
 *
 *    x = -0.149167, y = 0.850833
 *      has RGB 18, 38, 27
 *
 *    x = -0.135000, y = 0.870833
 *      has RGB 30, 64, 45
 *
 *    x = -0.106458, y = 0.883333
 *      has RGB 92, 192, 138
 *
 *    x = -0.1083333 y = 0.891667
 *      has RGB 70, 50, 230
 *
 */