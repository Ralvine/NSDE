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
#include <eigen3/Eigen/Dense>
#include <eigen3/Eigen/Core>
#include "Grid.h"
#include "Poisson.h"

class BVPSolver {
private:
    int m;
    int flag=0; // 求解标记
    Poisson &eq;
    Function &rhs, &bv;
    std::string BVType, DomainType;
    int Centerx, Centery, Radius;
    std::vector<double> ux, uy; // 存放解的点集
    std::vector<double> result;

public:
    BVPSolver(int _m, Poisson &_eq): m(_m), eq(_eq), rhs(_eq.getrhs()), bv(_eq.getbv()) {
        DomainType=eq.getDomainType();
        BVType=eq.getBVType();
        // std::cout << "构造成功!" << std::endl;
    }
    ~BVPSolver() {
        // std::cout << "析构成功!" << std::endl;
    }
    void solve() {
        Grid g(m, DomainType, BVPType);
        double h=1.0/(m+1);
        if(BVType=="Dirichlet") {
            Eigen::MatrixXd A(m*m,m*m);
            Eigen::MatrixXd B(m*m,1);
            for(int i=0; i<m*m; i++) {
                for(int j=0;j<m*m; j++) {
                    A(i,j)=0;
                }
            }
            if(DomainType=="D") {
                for(int k=0; k<m*m; k++) {
                    int tempi=(g.getPoint(k)).i;
                    int tempj=(g.getPoint(k)).j;
                    B(k,0)=rhs((g.getPoint(k)).x, (g.getPoint(k)).y);
                    if(tempi>1&&tempi<m&&tempj>1&&tempj<m) {
                        A(k,(g.getPoint(tempi,tempj)).num)=4.0/(h*h);
                        A(k,(g.getPoint(tempi-1,tempj)).num)=-1.0/(h*h);
                        A(k,(g.getPoint(tempi,tempj-1)).num)=-1.0/(h*h);
                        A(k,(g.getPoint(tempi,tempj+1)).num)=-1.0/(h*h);
                        A(k,(g.getPoint(tempi+1,tempj)).num)=-1.0/(h*h);
                    } else if(tempi==1&&tempj==1) {
                        A(k,(g.getPoint(tempi,tempj)).num)=4.0/(h*h);
                        A(k,(g.getPoint(tempi,tempj+1)).num)=-1.0/(h*h);
                        A(k,(g.getPoint(tempi+1,tempj)).num)=-1.0/(h*h);
                        B(k)+=(bv((g.getPoint(tempi-1,tempj)).x,(g.getPoint(tempi-1,tempj)).y)+bv((g.getPoint(tempi,tempj-1)).x,(g.getPoint(tempi,tempj-1)).y))/(h*h);
                    } else if(tempi==m&&tempj==m) {
                        A(k,(g.getPoint(tempi,tempj)).num)=4.0/(h*h);
                        A(k,(g.getPoint(tempi,tempj-1)).num)=-1.0/(h*h);
                        A(k,(g.getPoint(tempi-1,tempj)).num)=-1.0/(h*h);
                        B(k)+=(bv((g.getPoint(tempi+1,tempj)).x,(g.getPoint(tempi+1,tempj)).y)+bv((g.getPoint(tempi,tempj+1)).x,(g.getPoint(tempi,tempj+1)).y))/(h*h);
                    } else if(tempi==1&&tempj==m) {
                        A(k,(g.getPoint(tempi,tempj)).num)=4.0/(h*h);
                        A(k,(g.getPoint(tempi,tempj-1)).num)=-1.0/(h*h);
                        A(k,(g.getPoint(tempi+1,tempj)).num)=-1.0/(h*h);
                        B(k)+=(bv((g.getPoint(tempi-1,tempj)).x,(g.getPoint(tempi-1,tempj)).y)+bv((g.getPoint(tempi,tempj+1)).x,(g.getPoint(tempi,tempj+1)).y))/(h*h);
                    } else if(tempi==m&&tempj==1) {
                        A(k,(g.getPoint(tempi,tempj)).num)=4.0/(h*h);
                        A(k,(g.getPoint(tempi,tempj+1)).num)=-1.0/(h*h);
                        A(k,(g.getPoint(tempi-1,tempj)).num)=-1.0/(h*h);
                        B(k)+=(bv((g.getPoint(tempi+1,tempj)).x,(g.getPoint(tempi+1,tempj)).y)+bv((g.getPoint(tempi,tempj-1)).x,(g.getPoint(tempi,tempj-1)).y))/(h*h);
                    } else if(tempi>1&&tempi<m&&tempj==1) {
                        A(k,(g.getPoint(tempi,tempj)).num)=4.0/(h*h);
                        A(k,(g.getPoint(tempi,tempj+1)).num)=-1.0/(h*h);
                        A(k,(g.getPoint(tempi+1,tempj)).num)=-1.0/(h*h);
                        A(k,(g.getPoint(tempi-1,tempj)).num)=-1.0/(h*h);
                        B(k)+=(bv((g.getPoint(tempi,tempj-1)).x,(g.getPoint(tempi,tempj-1)).y))/(h*h);
                    } else if(tempi>1&&tempi<m&&tempj==m) {
                        A(k,(g.getPoint(tempi,tempj)).num)=4.0/(h*h);
                        A(k,(g.getPoint(tempi,tempj-1)).num)=-1.0/(h*h);
                        A(k,(g.getPoint(tempi+1,tempj)).num)=-1.0/(h*h);
                        A(k,(g.getPoint(tempi-1,tempj)).num)=-1.0/(h*h);
                        B(k)+=(bv((g.getPoint(tempi,tempj+1)).x,(g.getPoint(tempi,tempj+1)).y))/(h*h);
                    } else if(tempj>1&&tempj<m&&tempi==1) {
                        A(k,(g.getPoint(tempi,tempj)).num)=4.0/(h*h);
                        A(k,(g.getPoint(tempi,tempj+1)).num)=-1.0/(h*h);
                        A(k,(g.getPoint(tempi+1,tempj)).num)=-1.0/(h*h);
                        A(k,(g.getPoint(tempi,tempj-1)).num)=-1.0/(h*h);
                        B(k)+=(bv((g.getPoint(tempi-1,tempj)).x,(g.getPoint(tempi-1,tempj)).y))/(h*h);
                    } else if(tempj>1&&tempj<m&&tempi==m) {
                        A(k,(g.getPoint(tempi,tempj)).num)=4.0/(h*h);
                        A(k,(g.getPoint(tempi,tempj+1)).num)=-1.0/(h*h);
                        A(k,(g.getPoint(tempi,tempj-1)).num)=-1.0/(h*h);
                        A(k,(g.getPoint(tempi-1,tempj)).num)=-1.0/(h*h);
                        B(k)+=(bv((g.getPoint(tempi+1,tempj)).x,(g.getPoint(tempi+1,tempj)).y))/(h*h);
                    } else {
                        std::cerr << "运算越界!" << std::endl;
                        exit(0);
                    }
                }
            } else if(DomainType=="Omega") {
                Eigen::MatrixXd A(m*m,m*m);
                Eigen::MatrixXd B(m*m,1);
                for(int i=0; i<m*m; i++) {
                    for(int j=0;j<m*m; j++) {
                        A(i,j)=0;
                    }
                }
                for(int k=0; k<m*m; k++) {
                    int tempi=(g.getPoint(k)).i;
                    int tempj=(g.getPoint(k)).j;
                    B(k,0)=rhs((g.getPoint(k)).x, (g.getPoint(k)).y);
                    if(tempi>1&&tempi<m&&tempj>1&&tempj<m) {
                        A(k,(g.getPoint(tempi,tempj)).num)=4.0/(h*h);
                        A(k,(g.getPoint(tempi-1,tempj)).num)=-1.0/(h*h);
                        A(k,(g.getPoint(tempi,tempj-1)).num)=-1.0/(h*h);
                        A(k,(g.getPoint(tempi,tempj+1)).num)=-1.0/(h*h);
                        A(k,(g.getPoint(tempi+1,tempj)).num)=-1.0/(h*h);
                    } else if(tempi==1&&tempj==1) {
                        A(k,(g.getPoint(tempi,tempj)).num)=4.0/(h*h);
                        A(k,(g.getPoint(tempi,tempj+1)).num)=-1.0/(h*h);
                        A(k,(g.getPoint(tempi+1,tempj)).num)=-1.0/(h*h);
                        B(k)+=(bv((g.getPoint(tempi-1,tempj)).x,(g.getPoint(tempi-1,tempj)).y)+bv((g.getPoint(tempi,tempj-1)).x,(g.getPoint(tempi,tempj-1)).y))/(h*h);
                    } else if(tempi==m&&tempj==m) {
                        A(k,(g.getPoint(tempi,tempj)).num)=4.0/(h*h);
                        A(k,(g.getPoint(tempi,tempj-1)).num)=-1.0/(h*h);
                        A(k,(g.getPoint(tempi-1,tempj)).num)=-1.0/(h*h);
                        B(k)+=(bv((g.getPoint(tempi+1,tempj)).x,(g.getPoint(tempi+1,tempj)).y)+bv((g.getPoint(tempi,tempj+1)).x,(g.getPoint(tempi,tempj+1)).y))/(h*h);
                    } else if(tempi==1&&tempj==m) {
                        A(k,(g.getPoint(tempi,tempj)).num)=4.0/(h*h);
                        A(k,(g.getPoint(tempi,tempj-1)).num)=-1.0/(h*h);
                        A(k,(g.getPoint(tempi+1,tempj)).num)=-1.0/(h*h);
                        B(k)+=(bv((g.getPoint(tempi-1,tempj)).x,(g.getPoint(tempi-1,tempj)).y)+bv((g.getPoint(tempi,tempj+1)).x,(g.getPoint(tempi,tempj+1)).y))/(h*h);
                    } else if(tempi==m&&tempj==1) {
                        A(k,(g.getPoint(tempi,tempj)).num)=4.0/(h*h);
                        A(k,(g.getPoint(tempi,tempj+1)).num)=-1.0/(h*h);
                        A(k,(g.getPoint(tempi-1,tempj)).num)=-1.0/(h*h);
                        B(k)+=(bv((g.getPoint(tempi+1,tempj)).x,(g.getPoint(tempi+1,tempj)).y)+bv((g.getPoint(tempi,tempj-1)).x,(g.getPoint(tempi,tempj-1)).y))/(h*h);
                    } else if(tempi>1&&tempi<m&&tempj==1) {
                        A(k,(g.getPoint(tempi,tempj)).num)=4.0/(h*h);
                        A(k,(g.getPoint(tempi,tempj+1)).num)=-1.0/(h*h);
                        A(k,(g.getPoint(tempi+1,tempj)).num)=-1.0/(h*h);
                        A(k,(g.getPoint(tempi-1,tempj)).num)=-1.0/(h*h);
                        B(k)+=(bv((g.getPoint(tempi,tempj-1)).x,(g.getPoint(tempi,tempj-1)).y))/(h*h);
                    } else if(tempi>1&&tempi<m&&tempj==m) {
                        A(k,(g.getPoint(tempi,tempj)).num)=4.0/(h*h);
                        A(k,(g.getPoint(tempi,tempj-1)).num)=-1.0/(h*h);
                        A(k,(g.getPoint(tempi+1,tempj)).num)=-1.0/(h*h);
                        A(k,(g.getPoint(tempi-1,tempj)).num)=-1.0/(h*h);
                        B(k)+=(bv((g.getPoint(tempi,tempj+1)).x,(g.getPoint(tempi,tempj+1)).y))/(h*h);
                    } else if(tempj>1&&tempj<m&&tempi==1) {
                        A(k,(g.getPoint(tempi,tempj)).num)=4.0/(h*h);
                        A(k,(g.getPoint(tempi,tempj+1)).num)=-1.0/(h*h);
                        A(k,(g.getPoint(tempi+1,tempj)).num)=-1.0/(h*h);
                        A(k,(g.getPoint(tempi,tempj-1)).num)=-1.0/(h*h);
                        B(k)+=(bv((g.getPoint(tempi-1,tempj)).x,(g.getPoint(tempi-1,tempj)).y))/(h*h);
                    } else if(tempj>1&&tempj<m&&tempi==m) {
                        A(k,(g.getPoint(tempi,tempj)).num)=4.0/(h*h);
                        A(k,(g.getPoint(tempi,tempj+1)).num)=-1.0/(h*h);
                        A(k,(g.getPoint(tempi,tempj-1)).num)=-1.0/(h*h);
                        A(k,(g.getPoint(tempi-1,tempj)).num)=-1.0/(h*h);
                        B(k)+=(bv((g.getPoint(tempi+1,tempj)).x,(g.getPoint(tempi+1,tempj)).y))/(h*h);
                    } else {
                        std::cerr << "运算越界!" << std::endl;
                        exit(0);
                    }
                }
            }
        } else if(BVType=="Neumman") {
            if(DomainType=="D") {
                Eigen::MatrixXd A(m*m,m*m);
                Eigen::MatrixXd B(m*m,1);
                for(int i=0; i<m*m; i++) {
                    for(int j=0;j<m*m; j++) {
                        A(i,j)=0;
                    }
                }
                for(int k=0; k<m*m; k++) {
                    int tempi=(g.getPoint(k)).i;
                    int tempj=(g.getPoint(k)).j;
                    B(k,0)=rhs((g.getPoint(k)).x, (g.getPoint(k)).y);
                    if(tempi>1&&tempi<m&&tempj>1&&tempj<m) {
                        A(k,(g.getPoint(tempi,tempj)).num)=4.0/(h*h);
                        A(k,(g.getPoint(tempi-1,tempj)).num)=-1.0/(h*h);
                        A(k,(g.getPoint(tempi,tempj-1)).num)=-1.0/(h*h);
                        A(k,(g.getPoint(tempi,tempj+1)).num)=-1.0/(h*h);
                        A(k,(g.getPoint(tempi+1,tempj)).num)=-1.0/(h*h);
                    } else if(tempi==1&&tempj==1) {
                        A(k,(g.getPoint(tempi,tempj)).num)=4.0/(h*h);
                        A(k,(g.getPoint(tempi,tempj+1)).num)=-1.0/(h*h);
                        A(k,(g.getPoint(tempi+1,tempj)).num)=-1.0/(h*h);
                        B(k)+=(bv((g.getPoint(tempi-1,tempj)).x,(g.getPoint(tempi-1,tempj)).y)+bv((g.getPoint(tempi,tempj-1)).x,(g.getPoint(tempi,tempj-1)).y))/(h*h);
                    } else if(tempi==m&&tempj==m) {
                        A(k,(g.getPoint(tempi,tempj)).num)=4.0/(h*h);
                        A(k,(g.getPoint(tempi,tempj-1)).num)=-1.0/(h*h);
                        A(k,(g.getPoint(tempi-1,tempj)).num)=-1.0/(h*h);
                        B(k)+=(bv((g.getPoint(tempi+1,tempj)).x,(g.getPoint(tempi+1,tempj)).y)+bv((g.getPoint(tempi,tempj+1)).x,(g.getPoint(tempi,tempj+1)).y))/(h*h);
                    } else if(tempi==1&&tempj==m) {
                        A(k,(g.getPoint(tempi,tempj)).num)=4.0/(h*h);
                        A(k,(g.getPoint(tempi,tempj-1)).num)=-1.0/(h*h);
                        A(k,(g.getPoint(tempi+1,tempj)).num)=-1.0/(h*h);
                        B(k)+=(bv((g.getPoint(tempi-1,tempj)).x,(g.getPoint(tempi-1,tempj)).y)+bv((g.getPoint(tempi,tempj+1)).x,(g.getPoint(tempi,tempj+1)).y))/(h*h);
                    } else if(tempi==m&&tempj==1) {
                        A(k,(g.getPoint(tempi,tempj)).num)=4.0/(h*h);
                        A(k,(g.getPoint(tempi,tempj+1)).num)=-1.0/(h*h);
                        A(k,(g.getPoint(tempi-1,tempj)).num)=-1.0/(h*h);
                        B(k)+=(bv((g.getPoint(tempi+1,tempj)).x,(g.getPoint(tempi+1,tempj)).y)+bv((g.getPoint(tempi,tempj-1)).x,(g.getPoint(tempi,tempj-1)).y))/(h*h);
                    } else if(tempi>1&&tempi<m&&tempj==1) {
                        A(k,(g.getPoint(tempi,tempj)).num)=4.0/(h*h);
                        A(k,(g.getPoint(tempi,tempj+1)).num)=-1.0/(h*h);
                        A(k,(g.getPoint(tempi+1,tempj)).num)=-1.0/(h*h);
                        A(k,(g.getPoint(tempi-1,tempj)).num)=-1.0/(h*h);
                        B(k)+=(bv((g.getPoint(tempi,tempj-1)).x,(g.getPoint(tempi,tempj-1)).y))/(h*h);
                    } else if(tempi>1&&tempi<m&&tempj==m) {
                        A(k,(g.getPoint(tempi,tempj)).num)=4.0/(h*h);
                        A(k,(g.getPoint(tempi,tempj-1)).num)=-1.0/(h*h);
                        A(k,(g.getPoint(tempi+1,tempj)).num)=-1.0/(h*h);
                        A(k,(g.getPoint(tempi-1,tempj)).num)=-1.0/(h*h);
                        B(k)+=(bv((g.getPoint(tempi,tempj+1)).x,(g.getPoint(tempi,tempj+1)).y))/(h*h);
                    } else if(tempj>1&&tempj<m&&tempi==1) {
                        A(k,(g.getPoint(tempi,tempj)).num)=4.0/(h*h);
                        A(k,(g.getPoint(tempi,tempj+1)).num)=-1.0/(h*h);
                        A(k,(g.getPoint(tempi+1,tempj)).num)=-1.0/(h*h);
                        A(k,(g.getPoint(tempi,tempj-1)).num)=-1.0/(h*h);
                        B(k)+=(bv((g.getPoint(tempi-1,tempj)).x,(g.getPoint(tempi-1,tempj)).y))/(h*h);
                    } else if(tempj>1&&tempj<m&&tempi==m) {
                        A(k,(g.getPoint(tempi,tempj)).num)=4.0/(h*h);
                        A(k,(g.getPoint(tempi,tempj+1)).num)=-1.0/(h*h);
                        A(k,(g.getPoint(tempi,tempj-1)).num)=-1.0/(h*h);
                        A(k,(g.getPoint(tempi-1,tempj)).num)=-1.0/(h*h);
                        B(k)+=(bv((g.getPoint(tempi+1,tempj)).x,(g.getPoint(tempi+1,tempj)).y))/(h*h);
                    } else {
                        std::cerr << "运算越界!" << std::endl;
                        exit(0);
                    }
                }
            } else if(DomainType=="Omega") {
                Eigen::MatrixXd A(m*m,m*m);
                Eigen::MatrixXd B(m*m,1);
                for(int i=0; i<m*m; i++) {
                    for(int j=0;j<m*m; j++) {
                        A(i,j)=0;
                    }
                }
                for(int k=0; k<m*m; k++) {
                    int tempi=(g.getPoint(k)).i;
                    int tempj=(g.getPoint(k)).j;
                    B(k,0)=rhs((g.getPoint(k)).x, (g.getPoint(k)).y);
                    if(tempi>1&&tempi<m&&tempj>1&&tempj<m) {
                        A(k,(g.getPoint(tempi,tempj)).num)=4.0/(h*h);
                        A(k,(g.getPoint(tempi-1,tempj)).num)=-1.0/(h*h);
                        A(k,(g.getPoint(tempi,tempj-1)).num)=-1.0/(h*h);
                        A(k,(g.getPoint(tempi,tempj+1)).num)=-1.0/(h*h);
                        A(k,(g.getPoint(tempi+1,tempj)).num)=-1.0/(h*h);
                    } else if(tempi==1&&tempj==1) {
                        A(k,(g.getPoint(tempi,tempj)).num)=4.0/(h*h);
                        A(k,(g.getPoint(tempi,tempj+1)).num)=-1.0/(h*h);
                        A(k,(g.getPoint(tempi+1,tempj)).num)=-1.0/(h*h);
                        B(k)+=(bv((g.getPoint(tempi-1,tempj)).x,(g.getPoint(tempi-1,tempj)).y)+bv((g.getPoint(tempi,tempj-1)).x,(g.getPoint(tempi,tempj-1)).y))/(h*h);
                    } else if(tempi==m&&tempj==m) {
                        A(k,(g.getPoint(tempi,tempj)).num)=4.0/(h*h);
                        A(k,(g.getPoint(tempi,tempj-1)).num)=-1.0/(h*h);
                        A(k,(g.getPoint(tempi-1,tempj)).num)=-1.0/(h*h);
                        B(k)+=(bv((g.getPoint(tempi+1,tempj)).x,(g.getPoint(tempi+1,tempj)).y)+bv((g.getPoint(tempi,tempj+1)).x,(g.getPoint(tempi,tempj+1)).y))/(h*h);
                    } else if(tempi==1&&tempj==m) {
                        A(k,(g.getPoint(tempi,tempj)).num)=4.0/(h*h);
                        A(k,(g.getPoint(tempi,tempj-1)).num)=-1.0/(h*h);
                        A(k,(g.getPoint(tempi+1,tempj)).num)=-1.0/(h*h);
                        B(k)+=(bv((g.getPoint(tempi-1,tempj)).x,(g.getPoint(tempi-1,tempj)).y)+bv((g.getPoint(tempi,tempj+1)).x,(g.getPoint(tempi,tempj+1)).y))/(h*h);
                    } else if(tempi==m&&tempj==1) {
                        A(k,(g.getPoint(tempi,tempj)).num)=4.0/(h*h);
                        A(k,(g.getPoint(tempi,tempj+1)).num)=-1.0/(h*h);
                        A(k,(g.getPoint(tempi-1,tempj)).num)=-1.0/(h*h);
                        B(k)+=(bv((g.getPoint(tempi+1,tempj)).x,(g.getPoint(tempi+1,tempj)).y)+bv((g.getPoint(tempi,tempj-1)).x,(g.getPoint(tempi,tempj-1)).y))/(h*h);
                    } else if(tempi>1&&tempi<m&&tempj==1) {
                        A(k,(g.getPoint(tempi,tempj)).num)=4.0/(h*h);
                        A(k,(g.getPoint(tempi,tempj+1)).num)=-1.0/(h*h);
                        A(k,(g.getPoint(tempi+1,tempj)).num)=-1.0/(h*h);
                        A(k,(g.getPoint(tempi-1,tempj)).num)=-1.0/(h*h);
                        B(k)+=(bv((g.getPoint(tempi,tempj-1)).x,(g.getPoint(tempi,tempj-1)).y))/(h*h);
                    } else if(tempi>1&&tempi<m&&tempj==m) {
                        A(k,(g.getPoint(tempi,tempj)).num)=4.0/(h*h);
                        A(k,(g.getPoint(tempi,tempj-1)).num)=-1.0/(h*h);
                        A(k,(g.getPoint(tempi+1,tempj)).num)=-1.0/(h*h);
                        A(k,(g.getPoint(tempi-1,tempj)).num)=-1.0/(h*h);
                        B(k)+=(bv((g.getPoint(tempi,tempj+1)).x,(g.getPoint(tempi,tempj+1)).y))/(h*h);
                    } else if(tempj>1&&tempj<m&&tempi==1) {
                        A(k,(g.getPoint(tempi,tempj)).num)=4.0/(h*h);
                        A(k,(g.getPoint(tempi,tempj+1)).num)=-1.0/(h*h);
                        A(k,(g.getPoint(tempi+1,tempj)).num)=-1.0/(h*h);
                        A(k,(g.getPoint(tempi,tempj-1)).num)=-1.0/(h*h);
                        B(k)+=(bv((g.getPoint(tempi-1,tempj)).x,(g.getPoint(tempi-1,tempj)).y))/(h*h);
                    } else if(tempj>1&&tempj<m&&tempi==m) {
                        A(k,(g.getPoint(tempi,tempj)).num)=4.0/(h*h);
                        A(k,(g.getPoint(tempi,tempj+1)).num)=-1.0/(h*h);
                        A(k,(g.getPoint(tempi,tempj-1)).num)=-1.0/(h*h);
                        A(k,(g.getPoint(tempi-1,tempj)).num)=-1.0/(h*h);
                        B(k)+=(bv((g.getPoint(tempi+1,tempj)).x,(g.getPoint(tempi+1,tempj)).y))/(h*h);
                    } else {
                        std::cerr << "运算越界!" << std::endl;
                        exit(0);
                    }
                }
            }
        }
        Eigen::MatrixXd U=A.colPivHouseholderQr().solve(B);
        for(int i=0; i<m*m; i++) {
            result.push_back(U(i));
        }
        for(int k=0; k<m*m; k++) {
            ux.push_back((g.getPoint(k)).x);
            uy.push_back((g.getPoint(k)).y);
        }
        flag=1;
        // std::cout << U <<std::endl;
    }
    void print() {
        if(flag==1) {
            std::cout << "[" << result[0];
            for(int i=1; i<m*m; i++) {
                std::cout << "," << result[i];
            }
            std::cout << "]." << std::endl;
        } else {
            std::cerr << "尚未使用.solve()求解!" << std::endl;
            exit(0);
        }
        
    }
    std::vector<double> getUx() {
        if(flag==0) {
            std::cerr << "尚未使用.solve()求解!" << std::endl;
            exit(0);
        }
        return ux;
    }
    std::vector<double> getUy() {
        if(flag==0) {
            std::cerr << "尚未使用.solve()求解!" << std::endl;
            exit(0);
        }
        return uy;
    }
    std::vector<double> getResult() {
        if(flag==0) {
            std::cerr << "尚未使用.solve()求解!" << std::endl;
            exit(0);
        }
        return result;
    }
};

#else
#endif