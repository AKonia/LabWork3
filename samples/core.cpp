#include <iostream>
#include <string>
#include "MathExpr.hpp"
using std::cout;
using std::endl;

int main()
{

    MathExpr exprCalc;
    string str = "2 * ((312+4.5) * sin(pi/2)) + cos(x)*sin(y)";
    try
    {
        exprCalc.loadExpr(str);
        exprCalc.loadVariable("x", 0);
        exprCalc.loadVariable("y", 3.14);
        cout << exprCalc.compute() << endl;
    }
    catch(MathExpr::ExprException &e)
    {
        cout << e.getMessage() << endl;
    }

    return 0;
}
