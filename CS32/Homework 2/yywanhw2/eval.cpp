//
//  eval.cpp
//  homework2
//
//  Created by Yuen Yee Wan on 1/31/16.
//  Copyright © 2016 yywan. All rights reserved.
//

#include "Map.h"
#include <iostream>
#include <string>
#include <stack>
#include <cctype>
#include <cassert>
using namespace std;

bool isOperator(char a)
{
    if (a == '+' || a == '-' || a == '*' || a == '/')
        return true;
    return false;
}

string noSpaces(string infix)
{
    string noSpaces;
    for (int k = 0; k < infix.size(); k++)
        if (infix[k] != ' ')
            noSpaces += infix[k];
    return noSpaces;
}

bool isValidCharacters(string infix)
{

    int optr = 0;
    int oprnd = 0;
    int space = 0;
    int prthsOpen = 0;
    int prthsClose = 0;
    for (int k = 0; k < infix.size(); k++)
    {
        if (isOperator(infix[k]))
            optr++;
        else if (infix[k] == '(')
            prthsOpen++;
        else if (infix[k] == ')')
            prthsClose++;
        else if (infix[k] == ' ')
            space++;
        else if (islower(infix[k]))
            oprnd++;
        else
            return false;
    }
    if (oprnd == optr+1 && space != infix.size() && prthsClose == prthsOpen)
        return true;
    return false;
}

bool isValidOrder(string infix)
{
    infix = noSpaces(infix);
    int prthsOpen = 0;
    int prthsClose = 0;
    
    // check if parentheses close before being opened
    for (int k = 0; k < infix.size(); k++)
    {
        if (infix[k] == '(')
            prthsOpen++;
        if (infix[k] == ')')
            prthsClose++;
        if (prthsOpen < prthsClose)
            return false;
    }
    
    // check if every operator is in valid position
    if (isOperator(infix[0]) || isOperator(infix[infix.size()]))
        return false;
    for (int k = 1; k < infix.size()-1; k++)
    {
        if (isOperator(infix[k]))
            if (!(islower(infix[k-1]) && islower(infix[k+1])) && // operand operator operand
                !(islower(infix[k-1]) && infix[k+1] == '(') && // operand operator (
                !(infix[k-1] == ')' && islower(infix[k+1])) && // ) operator operand
                !(infix[k-1] == ')' && infix[k+1] == '(')) // ) operator (
                    return false;
    }
    return true;
}

bool inMap(string infix, const Map& values)
{
    for (int k = 0; k < infix.size(); k++)
    {
        if (islower(infix[k]))
            if (!values.contains(infix[k]))
                return false;
    }
    return true;
}

bool precedence(char a, char b)
{
    if (a == '+' || a == '-')
        return true;
    if ((a == '*' || a == '/') && (b == '+' || b == '-'))
        return false;
    return false;
}

int evaluate(string infix, const Map& values, string& postfix, int& result)
{
    if (!isValidOrder(infix) || !isValidCharacters(infix))
        return 1;
    infix = noSpaces(infix);
    
    postfix = "";
    stack<char> optr;
    
    for (int k = 0; k < infix.size(); k++)
    {
        if (islower(infix[k]))
            postfix += infix[k];
        if (infix[k] == '(')
            optr.push(infix[k]);
        if (infix[k] == ')')
        {
            while (optr.top() != '(') {
                postfix += optr.top();
                optr.pop();
            }
            optr.pop();
        }
        if (isOperator(infix[k]))
        {
            while (!optr.empty() && (optr.top() != '(') && precedence(infix[k],optr.top()))
            {
                postfix += optr.top();
                optr.pop();
            }
            optr.push(infix[k]);
        }
    }
    while (!optr.empty())
    {
        postfix += optr.top();
        optr.pop();
    }
    
    if (!inMap(postfix, values))
        return 2;
    
    stack<char> oprnd;
    for (int k = 0; k < postfix.size(); k++)
    {
        int value;
        if (islower(postfix[k]))
        {
            values.get(postfix[k], value);
            oprnd.push(value);
        }
        else
        {
            int operand2 = oprnd.top();
            oprnd.pop();
            int operand1 = oprnd.top();
            oprnd.pop();
            int tempResult = 99999;
            switch (postfix[k])
            {
                case '+':
                    tempResult = operand1 + operand2;
                    break;
                case '-':
                    tempResult = operand1 - operand2;
                    break;
                case '*':
                    tempResult = operand1 * operand2;
                    break;
                case '/':
                    if (operand2 == 0)
                        return 3;
                    tempResult = operand1 / operand2;
                    break;
            }
            oprnd.push(tempResult);
        }
    }
    result = oprnd.top();
    return 0;
}