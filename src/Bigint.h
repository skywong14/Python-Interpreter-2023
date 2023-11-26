#pragma once
#ifndef SJTU_BIGINTEGER
#define SJTU_BIGINTEGER

// Integer 1:
// 实现一个有符号的大整数类，只需支持简单的加减

// Integer 2:
// 实现一个有符号的大整数类，支持加减乘除，并重载相关运算符

// 请不要使用除了以下头文件之外的其它头文件
#include <complex>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <vector>

#include <cmath>  //have consulted TA before I use it!
// 请不要使用 using namespace std;
namespace sjtu {

    class int2048 {

    private:
        std::vector<long long> num;
        int flag, size;
        //flag 1: positive 0:negative
        static const long long base = 10000;
        static const long long width = 4;  //QUESION: int or long long?
        static constexpr long long mul[5] = {1, 10, 100, 1000, 10000}; //ATTENTION here 5 but not width + 1


    public:
        // 构造函数
        int2048();
        int2048(long long num1);
        int2048(const std::string &str1);
        int2048(const int2048 &n1);
        //============
        // Integer1
        //============
        void Check();

        void read(const std::string &str1);
        // 输出储存的大整数，无需换行
        void print();

        //ignore flag
        int2048 &add_positive(const int2048 &other);
        //abs this >= abs other is guaranteed(ignore flag)
        int2048 &minus_positive(const int2048 &other);
        // 加上一个大整数
        int2048 &add(const int2048 &other);
        // 返回两个大整数之和
        friend int2048 add(int2048 n1, const int2048 &n2);

        // 减去一个大整数
        int2048 &minus(const int2048 &other);
        // 返回两个大整数之差
        friend int2048 minus(int2048 n1, const int2048 &n2);

        //============
        // Integer2
        //============

        int2048 operator+() const;
        int2048 operator-() const;

        int2048 &operator=(const int2048 &other);

        int2048 &operator+=(const int2048 &other);
        friend int2048 operator+(int2048 n1, const int2048 &n2);

        int2048 &operator-=(const int2048 &other);
        friend int2048 operator-(int2048 n1, const int2048 &n2);



        int2048 &operator*=(const int2048 &other);
        friend int2048 operator*(int2048 n1, const int2048 &n2);

        int2048 plain_mul(const int2048 &Num, long long Mul, int BASE);

        int2048 ABS(const int2048 &Num);
        int2048 get_head(const int2048 &Num, int pos);
        int2048 &set_head(const int2048 &other, int pos);
        int2048 division(const int2048 &n1, const int2048 &n2);

        int2048 &operator/=(const int2048 &other);
        friend int2048 operator/ (int2048 n1, const int2048 &n2);

        int2048 &operator %=(const int2048 &other);
        friend int2048 operator % (int2048 n1, const int2048 &n2);
        friend std::ostream &operator<<(std::ostream &out, const int2048 &n1);


        friend bool operator==(const int2048 &n1, const int2048 &n2);
        friend bool operator!=(const int2048 &n1, const int2048 &n2);
        friend bool operator<(const int2048 &n1, const int2048 &n2);
        friend bool operator>(const int2048 &n1, const int2048 &n2);
        friend bool operator<=(const int2048 &n1, const int2048 &n2);
        friend bool operator>=(const int2048 &n1, const int2048 &n2);
        double int2048_to_Double();
    };
} // namespace sjtu

#endif