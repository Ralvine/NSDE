/**
 * @file   Grid.h
 * @author Ralvine(ralvine@163.com)
 * @date   The March 15 22:03:12 2023
 *
 * @brief  网格的结构.主要标记各网格点是否属于求解域、是否属于某种独特的边界（0-正方形左邻,1-正方形下邻,2-圆下邻.
 *
 *
 */

#ifndef __RALVINE_GRID__
#define __RALVINE_GRID__
#include <iostream>
#include <limits>
#include <vector>
#define Pi acos(-1)
#define MAXN 300
const double eps = std::numeric_limits<float>::epsilon();


// 先不考虑带圆情况,重写一遍结构.

struct Point {
    int x,y; // 存储坐标
    int i,j; // 存储下标
    int h; // 存储间距
    int num; // 存储编号
    int domainjud, boundjud; // 存储是否属于求解域、是否属于某种独特的边界
    Point(int _i, int _j, int _h): i(_i), j(_j), h(_h) {
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
    double x, y, h;
    std::vector<std::vector<Point>> web; // 以二维向量存储所有网格点
    std::vector<Point> index; // 存储索引:编号->点
    
public:
    Grid(int _m): m(_m) {
        h=1/m;
        // 构造网格点
        for(int i=0; i<=m+1; i++) {
            std::vector<Point> temp;
            for(int j=0; j<=m+1; j++) {
                Point p(i,j,h);
                temp.push_back(p);
                // 正方形时直接进行编号
                p.domainjud=1;
                p.num=i*(m+2)+j;
                index.push_back(p);
            }
            web.push_back(temp);
        }
        // std::cout << "构造成功!" << std::endl;
    }
    ~Grid() {
        // std::cout << "析构成功!" << std::endl;
    }
    int getNum(int x) {
        return 1;
    }
    Point getPoint(int num) {
        // std::cout << num << "," << m << std::endl;
        if(num>m) {
            std::cerr << "编号越界!" << std::endl;
            exit(0);
        }
        return index[num];
    }
};

#else
#endif