// Copyright 2016, ver. 0.001 by AKonia

// Список замеченных проблем:
//    Операторы на конце т.е. 2+2+ или 2^2^ и др.
//    Избыточные скобки в начале
//    Отсутствие машины ОЖИДАНИЙ
//    Неоптимизированная модель работы с функциями
//    Отсутствие возможности добавлять пользовательские способы обработки(хотя бы функции) 
//          желательно переделать на шаблонную обработку, чтобы расширять модуль можно было когда угодно
//    Отсутствие возможности обработки рациональных чисел в чистом виде :(
//    Нет прямой возможности пользователю добавлять константы, так то можно, но пока не ясно какие ограничения применить к пользователю
//    Нет проверки действий на допустимость т.е. деление на ноль или логарифм от отрицательного
//    Желательно улучшить до полуабстрактного шаблона
//    Нет поддержки скобок отличных от () 

// Нужно:
//    Добавить возможность добавления многопараметрических функций в т.ч. с запятыми и др., 
//    Для унарных и бинарных нужна возможность дать псевдоним операции например add(arg1, arg2) -> + 
//    Нужна возможность шаблонной обработки т.е. можно загрузить шаблон "diff(const^expr)" -> diff(expr) * log(const) 
//          или даже что-то типо add(expr1, expr2) -> expr1 expr2 call(add) где call(add) это уже прямой вызов функции 
//          т.е. опять же для diff(const) -> 0 или прошлое выражение -> log(const) * expr call(diff) 
//          т.е. почти прямо на ходу мы будем преобразовывать наше выражение в нечто похожее простое выражение содержащее обычные выражения с арифметикой простыми функциями
//          и так делать нужно рекурсивно пока на получим константу например
//    Очевидно из предыдущего что мы должны уметь вести библиотеку типов и уметь их распознавать т.е. число или функция и 
//          уметь с ними работать скорее всего обрабатывать кусок пока мы однозначно не выясним какого типа выражение
//          т.е. 2.102e+10 + 3, будет сначала обрабатываться как либо int || float || double затем встретив 
//          точку мы исключим int затем float как встретим e, встретив пробел закончим выражение но продолжим выражение т.е. оно преобразуется в expr(double function int), так 
//          так мы сможем потом оценить это чудо при обработке как производную, также предлагаю функцию производной оформить как  diff(expr, ... ) где ... это список 
//          объектов типа переменная и переменные отныне должны порождаться классом например variable(string name) и передавать можно будет так:
//              strexpr = "diff(x^2+x+1, x)";
//              x_var = variable("x");
//              expr.loadExpr(strexpt); 
//              expr.loadVariable(x_var);
//              expr.compute() // вернёт MathExpr = "2*x + 1"
//              // полноценные вычисления произойдут в случае если переменная имела значение 
//    То есть как видно из предыдущего примера нужно уметь работать с абстрактными выражениями


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
    if(translated)
        return;
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
    
    //if(exprOperators.top() == "(")
    //    throw ExprException("No operators after OPEN_BRACKET, possibly too much brackets",__LINE__);
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
