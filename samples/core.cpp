#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>
#include "MathExpr.hpp"

using std::cout;
using std::endl;
using std::cin;
using namespace cv;

#define WIDTH 800
int main(int argc, char * argv[])
{
    MathExpr exprCalc;
    string str = "2 * ((312+4.5) * sin(pi/2)) + cos(x)*sin(y)";
    MathExpr by_x, by_y;

    Mat image = Mat::zeros(WIDTH, WIDTH, CV_8UC3);
    double delta = M_PI/16.0;

    line( image, Point(0, WIDTH/2), Point(WIDTH, WIDTH/2), Scalar( 0 , 0, 255 ),  2, 1);
    line( image, Point(WIDTH/2, 0), Point(WIDTH/2, WIDTH), Scalar( 0 , 255, 0 ),  2, 1);
    cout << "Please enter f(x): " << endl;

    //str = "25*x * sin(x)";
    str = "10*x";
    //cin >> str;
    by_x.loadExpr(string(str));
    //str = "(-25)*x * cos(x)";
    if(argc > 1)
        str = string(argv[1]);
    else
        return -1;
    cout << str;
    by_y.loadExpr(string(str));
    try
    {
        for(double a = -M_PI * 15.0; a < M_PI * 15.0; a += delta)
        {
            by_x.loadVariable("x", a);
            by_y.loadVariable("x", a);

            Point p1((int)(WIDTH/2 + by_x.compute()), (int)(-15*by_y.compute()) + WIDTH/2);

            by_x.loadVariable("x", a + delta);
            by_y.loadVariable("x", a + delta);

            Point p2((int)(WIDTH/2 + by_x.compute()), (int)(-15*by_y.compute()) + WIDTH/2);

            line( image, p1, p2, Scalar( 255, 127, 0),  3, 16);
            imshow(str,image);
        }
    }
    catch(MathExpr::ExprException &e)
    {
        cout << e.getMessage() << endl;
        return -1;
    }
    if(argc > 2)
        str = string(argv[2]);
    else
        return -1;
    //str = "2 * ((312+4.5) * sin(pi/2)) + cos(x)*sin(y)";
    try
    {
        exprCalc.loadExpr(str);
        exprCalc.loadVariable("x", 0);
        exprCalc.loadVariable("y", 3.14);
        cout << endl << str << " = " << exprCalc.compute() << endl;
    }
    catch(MathExpr::ExprException &e)
    {
        cout << e.getMessage() << endl;
        return -1;
    }
    waitKey(0);
    return 0;
}
