// Copyright 2016, ver. 0.001 by AKonia

#ifndef __MATHEXPR_HPP__
#define __MATHEXPR_HPP__

#include "LexUtility.hpp"

#include <stack>
#include <queue>
#include <vector>

#include <string>

#include <cstring>
#include <cmath>

using std::stack;
using std::string;
using std::vector;
using std::queue;

class MathExpr
{
public: // Обслуживание
    class ExprException
    {
        string fullMsg;
    public:
        ExprException(string msg, int line)
        {
            fullMsg = "On line " + itos(line) + " were thrown exception: " + msg;
        }
        string getMessage()
        {
            return fullMsg;
        }
    };
private: // Данные с которыми работает вид
    string expr;
    bool exprValidationStatus;
    bool translated;
    bool isTranslated;
    //vector
    vector<Variable> variables;
    vector<bool> isConstant;
public: // Основные функции, с помощью которых происходит взаимотействие из вне
    MathExpr()
    {
        translated = false;
        addConstant("e", 2.71828);
    #ifdef M_PI
        addConstant("pi", M_PI);
    #else
        addConstant("pi", 3.14159);
    #endif

    }
    string getExpr()
    {
        return expr;
    }
    void loadExpr(const string &expr);
    void loadVariable(string name, double value)
    {
        for(int i = 0; i < variables.size(); i++)
            if(name == variables[i].name)
                if(!isConstant[i])
                {
                    variables[i].value = value;
                    return;
                }
                else
                    throw ExprException("Impossible to write variable value, it conflicting with constant name: '" + name + "'", __LINE__);
        addVariable(name, value);
    }
    void translate();
    double compute();
    bool isValid();
private:
    double getValueByName(string name)
    {
        for(int i = 0; i < variables.size(); i++)
            if(variables[i].name == name)
                return variables[i].value;
        throw ExprException("Can't find variable '" + name + "' value, possible it wasn't entered", __LINE__);
    }
    void addConstant(string name, double value)
    {
        if(detectTokType(name) == VARIABLE)
        {
            Variable var;
            var.name = name;
            var.value = value;
            variables.push_back(var);
            isConstant.push_back(true);
        }
        else
            throw ExprException("Can't add value because it isn't supported format of the variable", __LINE__);
    }
    void addVariable(string name, double value)
    {
        addConstant(name, value);
        isConstant[isConstant.size()-1] = false;
    }
};

void MathExpr::loadExpr(const string &expr)
{
    translated = false;
    if(expr != "")
        this->expr = expr;
    else
        exprValidationStatus = false;
}

void MathExpr::translate()
{
    expr = "(" + expr + ")";
    queue<string> tokens;
    string delimSequence = "";
    int wasDelim = 1;
    for(int i = 0; i < expr.length(); i++)
    {
        if(isspace(expr[i]))
            continue;
        if(strchr("+-*/()^|", expr[i]))
        {
            if(!wasDelim)
            {
                tokens.push(delimSequence);
                delimSequence = "";
            }
            wasDelim = 1;
            delimSequence += expr[i];
            tokens.push(delimSequence);
            delimSequence = "";
        }
        else
        {
            wasDelim = 0;
            delimSequence += expr[i];
        }
    }
    stack<string> exprOperators, values;
    LexType currType = UNKNOWN_TYPE,
            lastType = UNKNOWN_TYPE;
    do
    {
        currType = detectTokType(tokens.front()); // получили текущую лексему
        //cout << "Current token is: " << tokens.front() << endl;
        switch(currType) // в соответствии с типом лексемы выбираем что делать
        {
            case OPERATION:
                if((lastType == OPEN_BRACKET || lastType == UNKNOWN_TYPE) &&
                    (tokens.front() == "-" || tokens.front() == "+"))
                    values.push(string("0"));
                else if(lastType != VARIABLE && lastType != CLOSE_BRACKET && !isFunction(tokens.front()))
                    throw ExprException("Unexpected expression", __LINE__);

                if(!exprOperators.empty() && isHigherPriority(exprOperators.top(), tokens.front())) // если предыдущая операция имеет больший приоритет, то исполняем её
                {
                    if(isFunction(exprOperators.top()))
                    {
                        string temporary = values.top();
                        values.pop();
                        values.push(temporary + " " + exprOperators.top());
                    }
                    else
                    {
                        string right = values.top();
                        values.pop();
                        string left = values.top();
                        values.pop();
                        values.push(left + " " + right + " " + exprOperators.top());
                    }
                    exprOperators.pop();
                }

                lastType = OPERATION;
                exprOperators.push(tokens.front());
                break;

            case OPEN_BRACKET:
                if(lastType == VARIABLE)
                    throw ExprException("Bad expression: you can't miss operations before opening braces", __LINE__);
                lastType = OPEN_BRACKET;
                exprOperators.push(tokens.front());
                break;

            case CLOSE_BRACKET:
                lastType = CLOSE_BRACKET;
                if(exprOperators.empty())
                    throw ExprException("No operators before CLOSE_BRACKET, possibly too much brackets",__LINE__);
                while(detectTokType(exprOperators.top()) != OPEN_BRACKET)
                {
                    if(isFunction(exprOperators.top()))
                    {
                        string temporary = values.top();
                        values.pop();
                        values.push(temporary + " " + exprOperators.top());
                    }
                    else
                    {
                        string right = values.top();
                        values.pop();
                        string left = values.top();
                        values.pop();
                        values.push(left + " " + right + " " + exprOperators.top());
                    }
                    exprOperators.pop();
                }
                exprOperators.pop();
                break;
            case UNKNOWN_TYPE:
                throw ExprException("UNKNOWN_TYPE of lexem in expression", __LINE__);
                break;
            default:
                if(lastType == VARIABLE)
                    throw ExprException("NOT CORRECT DATA: VARIABLE AFTER VARIABLE", __LINE__);
                lastType = VARIABLE;
                values.push(tokens.front());
                break;
        }
        tokens.pop();
    }
    while(!tokens.empty());

    while(!exprOperators.empty() && !values.empty())
    {
        if(isFunction(exprOperators.top()))
        {
            string temporary = values.top();
            values.pop();
            values.push(temporary + " " + exprOperators.top());
        }
        else
        {
            string right = values.top();
            values.pop();
            string left = values.top();
            values.pop();
            values.push(left + " " + right + " " + exprOperators.top());
        }
        exprOperators.pop();
    }
    expr = values.top();
    translated = true;
    exprValidationStatus = true;
}

double MathExpr::compute()
{
    if(exprValidationStatus == false)
    {
        if(!translated)
            translate();
        if(exprValidationStatus == false)
            throw ExprException("Expression is not valid for computing", __LINE__);
    }
    queue<string> tokens;
    string delimSequence = "";
    int wasDelim = 1;
    for(int i = 0; i < expr.length(); i++)
    {
        if(isspace(expr[i]))
        {
            if(delimSequence == "")
                continue;
            tokens.push(delimSequence);
            delimSequence = "";
        }
        else
        {
            delimSequence += expr[i];
        }
    }
    if(delimSequence != "")
        tokens.push(delimSequence);
    stack<double> preValues;
    char ** passive = 0;
    double value = 0.0;
    while(!tokens.empty())
    {
        //cout << tokens.front() << endl;
        LexType currType = detectTokType(tokens.front());
        switch(currType)
        {
            case OPERATION:
                if(isFunction(tokens.front()))
                {
                    double singleValue = preValues.top();
                    preValues.pop();
                    preValues.push(calculateFunc(tokens.front(), singleValue, 0));
                }
                else
                {
                    double lhVal, rhVal;
                    rhVal = preValues.top();
                    preValues.pop();
                    lhVal = preValues.top();
                    preValues.pop();
                    preValues.push(calculateFunc(tokens.front(), lhVal, rhVal));
                }
                break;
            case INT_NUMBER:
            case REAL_NUMBER:
                value = strtod(tokens.front().c_str(), passive);
                preValues.push(value);
                break;
            case UNKNOWN_TYPE:
                throw ExprException("UNKNOWN_TYPE of expression", __LINE__);
                break;
            case CONSTANT:
            case VARIABLE:
                preValues.push(getValueByName(tokens.front()));
                break;
        }
        tokens.pop();
    }
    return preValues.top();
}

bool MathExpr::isValid()
{
    return exprValidationStatus;
}


#endif // __MATHEXPR_HPP__
