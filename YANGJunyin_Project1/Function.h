/**
 * @file   Function.h
 * @author Ralvine(ralvine@163.com)
 * @date   The March 15 10:16:33 2023
 *
 * @brief  函数类.存储泊松方程的右端f(x,y)函数及边值条件的u或u'.
 *
 *
 */

#ifndef __RALVINE_FUNCTION__
#define __RALVINE_FUNCTION__
#include <iostream>
#include <limits>
#define Pi acos(-1)

const double eps = std::numeric_limits<float>::epsilon();

class Function {
public:
    virtual double operator()(double x,double y)=0;
};

#else
#endif