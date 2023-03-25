/**
 * @file   BVPSolver.h
 * @author Ralvine(ralvine@163.com)
 * @date   The March 15 10:19:21 2023
 *
 * @brief  边值问题的求解器类.给定一个边值问题的泊松方程及其边值条件类型,输出相应的u求解结果.
 *
 *
 */

#ifndef __RALVINE_BVPSOLVER__
#define __RALVINE_BVPSOLVER__
#include <iostream>
#include <limits>
#include <vector>
#include "Grid.h"

class BVPSOLVER {
public:
    virtual double operator()(double x,double y)=0;
};

#else
#endif