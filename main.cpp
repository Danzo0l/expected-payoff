#include <iostream>
#include <vector>
#include <random>
#include <math.h>
#include <ctime>


struct params
{
    // save all values in vector
    bool indexing = false; 
    // set height & widgth
    float height = 1.0;
    // count of control points
    int points = 100;
    // run mode: true - full passage, false - random values
    bool mode = false;
    // count of test (only for random values)
    int count_tests = 8192;
};

struct point
{
    float x;
    float y;
};

// program configs
params prefs;
// areas of triangles
std::vector<float> analistyc;
// test counter
int count = 0;
// sum of area of triangles
long double summ;


// calculating the area of a triangle
float triangle_area(point a, point b, point c);
// set 0 for analytical variables
void set_null();
// random number generation from 0 to b
float generate_random(float b);
// test logging
void logging(float area);
// TEST: full enumeration of all point values 
void theoretical_test();
// TEST: pseudo random point values
void pseudo_real_test();


int main() {
    srand(time(NULL));

    set_null();

    if (prefs.mode) {
        theoretical_test();
    } else {
        pseudo_real_test();
    }

    // print expected payoff area
    std::cout << "Expected payoff: \x1B[33m" << summ / count << "\033[0m" << std::endl;

    return 0;
};


float triangle_area(point a, point b, point c) {
    return 0.5 * abs((b.x - a.x) * (c.y - a.y) - (c.x - a.x) * (b.y - a.y));
};


void set_null() {
    summ = 0;
    count = 0;
    analistyc.clear();
}



float generate_random(float b) {
    // generate whole part 
    int number = ((int)(rand()) % int(b));
    // generate fractional part 
    float fractional =(double)(rand()) / RAND_MAX; 
    // return sum whoole and fractional parts
    return number + fractional;
};


void logging(float area) {
    if (prefs.indexing) {
        // add value to vector
        analistyc.push_back(area);
    }
};


void theoretical_test() {
    for (float x1=0; x1< prefs.height; x1+=(prefs.height/prefs.points)) {
        for (float y1=0; y1< prefs.height; y1+=(prefs.height/prefs.points)) {
            for (float x2=0; x2< prefs.height; x2+=(prefs.height/prefs.points)) {
                for (float y2=0; y2< prefs.height; y2+=(prefs.height/prefs.points)) {
                    // check points positions: 1st point in up area, 2nd point in low area
                    if ((x1 > y1) && (x2 < y2)) {
                        // generate 1 variant of points
                        // initialization points
                        point a, b, c;
                        a.x = x1;
                        a.y = y1;
                        b.x = x2;
                        b.y = y2;
                        // 3rb point is a const
                        c.x = 0;
                        c.y = 0;
                        // calculate area
                        float area = triangle_area(a, b, c);
                        // calculate areas sum
                        summ += area;
                        // save areas in vector
                        logging(area);
                        // count test
                        count++;
                    }
                }
            }
        }
    }
};


void pseudo_real_test() {
    for (int i=0; i<prefs.count_tests; i++) {
        // generate random points
        // initialization points
        point a, b, c;
        a.x = generate_random(prefs.height);
        a.y = generate_random(prefs.height);
        b.x = generate_random(prefs.height);
        b.y = generate_random(prefs.height);
        // 3rb point is a const
        c.x = 0;
        c.y = 0;
        // check points positions: 1st point in up area, 2nd point in low area
        if ((a.x > a.y) && (b.x < b.y)) {
            // calculate area
            float area = triangle_area(a, b, c);
            // calculate areas sum
            summ += area;
            // save areas in vector
            logging(area);
            // count test
            count++;
        }
    }
};