/**
 * @file   Grid.h
 * @author Ralvine(ralvine@163.com)
 * @date   The March 15 22:03:12 2023
 *
 * @brief  网格的结构,包括网格点的Struct和整个网格类.主要标记各网格点是否属于求解域/是否属于某种独特的边界.
 *
 *
 */

#ifndef __RALVINE_GRID__
#define __RALVINE_GRID__
#include <iostream>
#include <limits>
#include <vector>
#define MAXN 300


// 先不考虑带圆情况,重写一遍结构.

struct Point {
    double x,y; // 存储坐标
    int i,j; // 存储下标
    double h; // 存储间距
    int num; // 存储编号
    int domainjud=0;
    int boundjud=0; // 存储是否属于求解域、是否属于某种独特的边界
    Point(int _i, int _j, double _h): i(_i), j(_j), h(_h) {
        x=i*h;
        y=j*h;
    }
    void print() {
        std::cout << "该点下标为" << "[" << i << "," << j << "],";
        std::cout << "坐标为" << "[" << x << "," << y << "],";
        std::cout << "h=" << h << "." << std::endl;
    }
};

class Grid {
private:
    int m;
    std::string DomainType, BVPType;
    double h;
    std::vector<std::vector<Point>> web; // 以二维向量存储所有网格点
    std::vector<Point> index; // 存储索引:编号->点
    
public:
    Grid(int _m, std::string _DomainType, std::string _BVPType): m(_m), DomainType(_DomainType), BVPType(_BVPType) {
        h=1.0/(m+1);
        // std::cout << "h=" << h << std::endl;
        // 构造网格点
        int iterator=0;
        for(int j=0; j<=m+1; j++) {
            std::vector<Point> temp;
            for(int i=0; i<=m+1; i++) {
                Point p(i,j,h);
                // p.print();
                // 正方形时直接进行编号
                if(DomainType=="D" && BVPType=="Dirichlet") {
                    // D&Dirichlet采用内点编号
                    if(j>=1&&j<=m&&i>=1&&i<=m) {
                        p.domainjud=1; // 处于界内
                        p.num=iterator;
                        index.push_back(p);
                        iterator=iterator+1;
                    } else {
                        p.domainjud=0;
                        p.num=-1;
                    }
                    // std::cout << "i=" << i << ",j=" << j << ",编号=" << p.num << std::endl;
                } else if(DomainType=="Omega" && BVPType=="Dirichlet") {
                    // Thinking...
                } else {
                    std::cerr << "区域类型有误!" << std::endl;
                    exit(0);
                }
                temp.push_back(p);
            }
            web.push_back(temp);
        }
        // std::cout << "构造成功!" << std::endl;
        // for(int j=0; j<=m+1; j++) {
        //     for(int i=0; i<=m+1; i++) {
        //         std::cout << web[i][j].num << ",";
        //     }
        //     std::cout << std::endl;
        // }
    }
    ~Grid() {
        // std::cout << "析构成功!" << std::endl;
    }
    Point getPoint(int num) {
        // std::cout << num << "," << m << std::endl;
        if(DomainType=="D" && BVPType=="Dirichlet") {
            if(num>=m*m) {
                std::cerr << "编号越界!" << std::endl;
                exit(0);
            }
        } else if(DomainType=="D" && BVPType=="Neumman") {
            if(num>=(m+1)*(m+1)) {
                std::cerr << "编号越界!" << std::endl;
                exit(0);
            }
        } else if(DomainType=="Omega" && BVPType=="Dirichlet") {
            if(num>=m*m) {
                std::cerr << "编号越界!" << std::endl;
                exit(0);
            }
        } else if(DomainType=="Omega" && BVPType=="Neumman") {
            if(num>=(m+1)*(m+1)) {
                std::cerr << "编号越界!" << std::endl;
                exit(0);
            }
        } else {
            std::cerr << "区域或边值条件类型有误!" << std::endl;
            exit(0);
        }
        return index[num];
    }
    Point getPoint(int i, int j) {
        if(i>m+1||i<0||j>m+1||j<0) {
            std::cerr << "下标越界!" << std::endl;
            exit(0);
        }
        return web[i][j];
    }
};

#else
#endif