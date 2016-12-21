#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>
#include "MathExpr.hpp"

using std::cout;
using std::endl;
using namespace cv;

#define WIDTH 800
int main(int argc, char * argv[])
{
    MathExpr exprCalc;
    string str = "2 * ((312+4.5) * sin(pi/2)) + cos(x)*sin(y)";
    MathExpr by_x, by_y;
    
    str = "50*x";
    by_x.loadExpr(string(str));

    str = "-300 * cos(x)";

    by_y.loadExpr(string(str));

    Mat image = Mat::zeros(WIDTH, WIDTH, CV_8UC3);
    double delta = M_PI/16.0;

    line( image, Point(0, WIDTH/2), Point(WIDTH, WIDTH/2), Scalar( 0 , 0, 255 ),  2, 1);
    line( image, Point(WIDTH/2, 0), Point(WIDTH/2, WIDTH), Scalar( 0 , 255, 0 ),  2, 1);
    try
    {
        for(double a = 0.0; a < M_PI * 5.0; a += delta)
        {
            by_x.loadVariable("x", a);
            by_y.loadVariable("x", a);
   
            Point p1((int)(by_x.compute()), (int)(by_y.compute()) + WIDTH/2);

            by_x.loadVariable("x", a + delta);
            by_y.loadVariable("x", a + delta);

            Point p2((int)(by_x.compute()), (int)(by_y.compute()) + WIDTH/2);
    
            line( image, p1, p2, Scalar( 255, 127, 0),  2, 16);
            imshow(str,image);
        }
    }
    catch(MathExpr::ExprException &e)
    {
        cout << e.getMessage() << endl;
    }
    str = "2 * ((312+4.5) * sin(pi/2)) + cos(x)*sin(y)";
    try
    {
        exprCalc.loadExpr(str);
        exprCalc.loadVariable("x", 0);
        exprCalc.loadVariable("y", 3.14);
        cout << str << " = " << exprCalc.compute() << endl;
    }
    catch(MathExpr::ExprException &e)
    {
        cout << e.getMessage() << endl;
    }
    waitKey(0);
    return 0;
}
