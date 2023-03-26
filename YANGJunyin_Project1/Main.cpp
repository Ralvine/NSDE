/**
 * @file   test.cpp
 * @author Ralvine(ralvine@163.com)
 * @date   The March 22 23:01:23 2023
 *
 * @brief  用于计算和测试的主程序.
 *
 *
 */

#include <iostream>
#include <fstream>
#include <json/json.h>
#include <cmath>
#include "Function.h"
#include "Poisson.h"
#include "Grid.h"
#include "BVPSolver.h"
#include <eigen3/Eigen/Dense>
#include <eigen3/Eigen/Core>

class Rhsfunc1: public Function {
public:
    double operator()(double x, double y) {
        return (sin(x)*sin(x)+sin(x)-2)*exp(y+sin(x));
    }
};

class Realfunc1: public Function {
public:
    double operator()(double x, double y) {
        return exp(y+sin(x));
    }
};

class BVfunc1: public Function {
public:
    double operator()(double x, double y) {
        if(x==0&&y>=0&&y<=1) {
            return 0;
        } else if(x==1&&y>=0&&y<=1) {
            return 0;
        } else if(y==0&&x>=0&&x<=1) {
            return 0;
        } else if(y==1&&x>=0&&x<=1) {
            return 0;
        } else {
            return -1;
        }
    }
};


int main() {
    Json::Reader reader; Json::Value root;
    // 注意!由于开发时Mac环境限制,这里导入了文件的绝对路径,如需要测试,请自行修改为json的相对路径或新的绝对路径.
	std::ifstream in("./data/test1.json");
    std::string BVType, DomainType;
    int Center[2], Radius;
    if(reader.parse(in, root)) {
        BVType=root["BVType"].asString();
        DomainType=root["DomainType"].asString();
        if(DomainType=="Omega") {
            Center[0]=root["Center"][0].asInt();
            Center[1]=root["Center"][1].asInt();
            Radius=root["Radius"].asInt();
        }
    }

    Rhsfunc1 rhs1;
    BVfunc1 bv1;
    Realfunc1 u1;
    Poisson p1(rhs1, BVType, bv1, DomainType);
    int n1=8;
    int n2=16;
    int n3=32;
    int n4=64;
    BVPSolver s1(n1,p1);
    BVPSolver s2(n2,p1);
    BVPSolver s3(n3,p1);
    BVPSolver s4(n4,p1);

    s1.solve();
    std::vector<double> ux,uy;
    std::vector<double> realu;
    std::vector<double> calu;
    calu=s1.getResult();
    ux=s1.getUx();
    uy=s1.getUy();
    for(int i=0; i<n1*n1; i++) {
        realu.push_back(u1(ux[i],uy[i]));
    }
    double err1,err2;
    err1=0;
    for(int i=0; i<n1*n1; i++) {
        err1=err1+abs(calu[i]-realu[i]);
    }
    std::cout << "1-范数误差为" << err1 << std::endl;
    err2=0;
    for(int i=0; i<n1*n1; i++) {
        err2=err2+(calu[i]-realu[i])*(calu[i]-realu[i]);
    }
    err2=sqrt(err2);
    std::cout << "2-范数误差为" << err2 << std::endl;

    std::cout << "[" << realu[0];
    for(int i=1; i<n1*n1; i++) {
        std::cout << "," << realu[i];
    }
    std::cout << "]." << std::endl;


    std::cout << "[" << calu[0];
    for(int i=1; i<n1*n1; i++) {
        std::cout << "," << calu[i];
    }
    std::cout << "]." << std::endl;

    std::ofstream out;
    out.open("./output/test1.txt");
    std::cout << "[" << realu[0];
    for(int i=1; i<n1*n1; i++) {
        std::cout << "," << realu[i];
    }
    std::cout << "]." << std::endl;
    // s1.print();
    // s2.print();
    // s3.print();
    // s4.print();
    out.close();


    return 0;
}