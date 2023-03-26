/**
 * @file   test.cpp
 * @author Ralvine(ralvine@163.com)
 * @date   Unknown
 *
 * @brief  用于开发过程中进行测试的临时代码.
 *
 *
 */


// 目前考虑通过构造函数来生成f和边值函数,其余参数直接由json文件传入.


/* 然后进行 Grid 类的测试 */

#include<iostream>
#include "Grid.h"

int main() {
    Grid g(3);
    // std::cout << g.getPoint(2).num << std::endl; // 测试编号<->点的双射正确性
    return 0;
}




/* 测试网格点的数据结构 

#include <iostream>
#include <vector>
#include "Grid.h"

int main() {
    int m=10;
    std::vector<std::vector<Point>> web;
    for(int i=0; i<=m+1; i++) {
        std::vector<Point> temp;
        for(int j=0; j<=m+1; j++) {
            Point p(i,j,3);
            temp.push_back(p);
            // temp[j].print(); // 测试vector正确性及其输出.
        }
        web.push_back(temp);
    }
    // for(int i=0; i<=m+1; i++)
    //     for(int j=0; j<=m+1; j++)
    //         web[i][j].print();
    return 0;
}
*/

/* 传入参数,测试函数类及其泊松方程的生成. 

#include <iostream>
#include <fstream>
#include <json/json.h>
#include "Function.h"
#include "Poisson.h"

class Rhsfunc: public Function {
public:
    double operator()(double x, double y) {
        if(x==0&&y>=0&&y<=1) {
            return 0;
        } else if(x==1&&y>=0&&y<=1) {
            return 0;
        } else if(y==0&&x>=0&&x<=1) {
            return 0;
        } else if(y==1&&x>=0&&y<=1) {
            return 0;
        } else {
            return -1;
        }
    }
};

class BVfunc: public Function {
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
	std::ifstream in("/Users/ralvine/Documents/时下/YANGJunyin_Project1/test.json");
    std::string BVType, DomainType;
    int Center[2], Radius;
    if (reader.parse(in, root)) {
        BVType=root["BVType"].asString();
        DomainType=root["DomainType"].asString();
        Center[0]=root["Center"][0].asInt();
        Center[1]=root["Center"][1].asInt();
        Radius=root["Radius"].asInt();
    }
    // 构造泊松方程的RHS已知函数.
    Rhsfunc f1;
    BVfunc f2;
    // 构造泊松方程.
    // 测试字符串判定.
    // if(BVType=="Dirichlet") {
    //     std::cout << "NB!" << std::endl;
    // }
    std::cout << Center[0] << "," << Center[1] << std::endl;
    //Poisson p(f1, BVType, f2, DomainType, Center[0], Center[1], Radius);
    Poisson p(f1, BVType, f2, DomainType);
    p.print();
    // std::cout << f1(1,2) << std::endl;
    // std::cout << BVType << std::endl;
    // std::cout << f2(3,3) << std::endl;
    // std::cout << DomainType << std::endl;
    // std::cout << Radius << std::endl;
    return 0;
}

*/


/* 测试一下json输入.

#include <iostream>
#include <fstream>
#include <json/json.h>

int main() {
    Json::Reader reader; Json::Value root;
	std::ifstream in("/Users/ralvine/Documents/时下/YANGJunyin_Project1/test.json");
    std::string type;

    if (reader.parse(in, root)) {
        type=root["type"].asString();
    }
    if(type=="Dirichlet") {
        std::cout << "NB!" << std::endl;
    }
    return 0;
}
*/