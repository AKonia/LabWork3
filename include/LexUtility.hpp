#include <cctype>
#include <string>
#include <cstring>
#include <cmath>
#include <cstdlib>
#include <iostream>

using std::cout;
using std::endl;
using std::string;

const int qOfSupportedFunctions = 6;
const int qOfPriority = qOfSupportedFunctions + 5;
const char * operations = "+-*/%^";
const char * delims = "(){}[]|+-*/%^";
const char * openingBrackets = "([{|";
const char * closingBrackets = ")]}|";

string itos(unsigned int);
bool isVariable(char x);
bool isHigherPriority(const string &lhs, const string &rhs);
bool isFunction(string operation);

string functions[qOfSupportedFunctions] =
{
    string("sin"),
    string("cos"),
    string("tan"),
    string("log"),
    string("abs"),
    string("unm")
};

string priorityList[qOfPriority] =
{
    string("sin"),
    string("cos"),
    string("tan"),
    string("log"),
    string("abs"),
    string("unm"),
    string("^"),
    string("*"),
    string("/"),
    string("-"),
    string("+")
};

struct Variable
{
    string name;
    float value;
};

enum LexType
{
    UNKNOWN_TYPE = 0,
    VARIABLE,
    REAL_NUMBER,  // +
    INT_NUMBER,   // +
    CONSTANT,
    OPERATION,    // +
    OPEN_BRACKET, // +
    CLOSE_BRACKET // +
};

bool isVariable(char x)
{
    return (isalnum(x) || (x == '_') || (x == '$'));
}

string itos(unsigned int number)
{
    string result = "";
    string reversed = "";
    while(number != 0)
    {
        reversed += (char)('0' + number % 10);
        number /= 10;
    }
    for(int i = 0; i < reversed.length(); i++)
        result += reversed[i];
    return result;
}

bool isHigherPriority(const string & lhs, const string & rhs)
{
    for(int i = 0; i < qOfPriority; i++)
    {
        if(priorityList[i] == lhs)
            return true;
        if(priorityList[i] == rhs)
            return false;
    }
}

bool isFunction(string operation)
{
    for(int i = 0; i < qOfSupportedFunctions; i++)
        if(operation == functions[i])
            return true;
    return false;
}

LexType detectTokType(string tok)
{
    size_t toklen = tok.length();
    if(toklen < 1)
        return UNKNOWN_TYPE;
    if(toklen == 1)
    {
        if(strchr(operations, tok[0]))
            return OPERATION;
        if(strchr(openingBrackets, tok[0]))
            return OPEN_BRACKET;
        if(strchr(closingBrackets, tok[0]))
            return CLOSE_BRACKET;
        // if(tok[0] == 'e')
        //     return CONSTANT;
    }
    // if(tok == "pi")
    // {
    //     return CONSTANT;
    // }
    if(isdigit(tok[0]))
    {
        LexType numericType = INT_NUMBER;
        for(int i = 1; i < toklen; i++)
            if(!isdigit(tok[i]))
            {
                if(!((tok[i] == '.') || (tok[i] == ',')) || REAL_NUMBER == numericType)
                    return UNKNOWN_TYPE;
                else
                    numericType = REAL_NUMBER;
            }
        return numericType;
    }
    if(toklen == 3)
        for(int i = 0; i < 5; i++)
            if(tok == functions[i])
                return OPERATION;
    if(isalpha(tok[0]) || tok[0] == '_' || tok[0] == '$')
    {
        for(int i = 1; i < toklen; i++)
            if(!isVariable(tok[0]))
                return UNKNOWN_TYPE;
        return VARIABLE;
    }
    return UNKNOWN_TYPE;
}

bool findValByName(string name, double &value)
{
    value = 1.0;
    return true;
}
double calculateFunc(string funcName, double l, double r)
{
    if(!isFunction(funcName))
    {
        switch(funcName[0])
        {
            case '+':
                return l+r;
            case '-':
                return l-r;
            case '*':
                return l*r;
            case '/':
                return l / r;
            case '^':
                return pow(l, r);
        }
    }
    else
    {
        if(funcName == "sin")
            return sin(l);
        if(funcName == "cos")
            return cos(l);
        if(funcName == "abs")
            return abs(l);
        if(funcName == "log")
            return log(l);
        if(funcName == "tan")
            return tan(l);
    }
}
