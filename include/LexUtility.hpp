#include <cctype>
#include <string>
#include <cstring>
#include <cmath>
#include <cstdlib>

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
    string("abs")
};

string priorityList[qOfPriority] =
{
    string("sin"),
    string("cos"),
    string("tan"),
    string("log"),
    string("abs"),
    string("^"),
    string("/"),
    string("*"),
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
    REAL_NUMBER,
    INT_NUMBER,
    CONSTANT,
    OPERATION,
    OPEN_BRACKET,
    CLOSE_BRACKET
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
        if(priorityList[i] == rhs)
            return false;
        if(priorityList[i] == lhs)
            return true;
    }
}

bool isFunction(string operation)
{
    for(int i = 0; i < qOfSupportedFunctions; i++)
        if(operation == functions[i])
            return true;
    return false;
}

LexType detectTokType(const string tok)
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
    }
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
