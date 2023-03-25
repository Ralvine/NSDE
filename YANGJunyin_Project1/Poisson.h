/**
 * @file   Poisson.h
 * @author Ralvine(ralvine@163.com)
 * @date   The March 15 10:16:55 2023
 *
 * @brief  二维泊松方程类.包括泊松方程的f(x,y),边值条件类型,边值条件函数,区域类型,圆的.
 *
 *
 */

#ifndef __RALVINE_POISSON__
#define __RALVINE_POISSON__
#include <iostream>
#include "Function.h"

class Poisson {
private:
    Function &rhs, &bv;
    std::string BVType, DomainType;
    int Centerx, Centery, Radius;
public:
    Poisson(Function &_rhs, std::string _BVType, Function &_bv, std::string _DomainType, int _Centerx, int _Centery, int _Radius): 
    rhs(_rhs), BVType(_BVType), bv(_bv), DomainType(_DomainType), Centerx(_Centerx), Centery(_Centery),Radius(_Radius) {
        // std::cout << "构造成功！" << std::endl;
        if(DomainType!="Omega") {
            std::cerr << "输入的区域类型有误.\n";
            exit(0);
        }
    }
    Poisson(Function &_rhs, std::string _BVType, Function &_bv, std::string _DomainType): 
    rhs(_rhs), BVType(_BVType), bv(_bv), DomainType(_DomainType) {
        if(DomainType!="D") {
            std::cerr << "输入的区域类型有误.\n";
            exit(0);
        }
    }
    ~Poisson() {}
    void print() {
        std::cout << "BVType:" << BVType << std::endl;
        std::cout << "DomainType:" << DomainType << std::endl;
        if(DomainType=="Omega") {
            std::cout << "Center: [" << Centerx << "," << Centery << "]" << std::endl;
            std::cout << "Radius:" << Radius << std::endl;
        }
    }
};


#else
#endif