//OPERATOR:any
#ifndef SKYW_ANY
#define SKYW_ANY
#pragma once
#include <any>
#include <iostream>
#include <cmath>
#include <cstring>
#define Int long long

//供测试用
double Int_to_Double(long long x);
long long String_to_Int(std::string x);

//关于变量
bool is_variable(std::any const &a);

//解除Tuple
void release_Var(std::any &a);
//判断类型
bool is_Int(std::any const &a);
bool is_Double(std::any const &a);
bool is_Bool(std::any const &a);
bool is_String(std::any const &a);
bool is_Integer(std::any const &a);
bool is_Number(std::any const &a);
bool is_Tuple(std::any const &a);
bool is_None(std::any const &a);

//类型转换
Int to_Int(std::any const &a);
double to_Double(std::any const &a);
std::string to_String(std::any const &a);
bool to_Bool(std::any const &a);
std::any operator+(std::any const &a1,std::any const &a2);
std::any operator-(std::any const &a1,std::any const &a2);
std::any operator*(std::any const &a1,std::any const &a2);
std::any operator/(std::any const &a1,std::any const &a2);
std::any operator-(std::any const &a1);
std::any DivInt(std::any const &a1,std::any const &a2);
std::any DivDouble(std::any const &a1,std::any const &a2);
std::any operator%(std::any const &a, std::any const &b); //TODO 待实现
// 比较运算符
bool operator==(std::any const &a1,std::any const &a2);
bool operator<(std::any const &a1,std::any const &a2);
bool operator>(std::any const &a1,std::any const &a2);
bool operator>=(std::any const &a1, std::any const &a2);
bool operator<=(std::any const &a1, std::any const &a2);
bool operator!=(std::any const &a1, std::any const &a2);
//AugAssign
std::any& operator+=(std::any &a1, std::any const &a2);
std::any& operator-=(std::any &a1, std::any const &a2);
std::any& operator*=(std::any &a1, std::any const &a2);
std::any& operator%=(std::any &a1, std::any const &a2); //TODO 待实现
//输出流
std::ostream &operator<<(std::ostream &os, std::any const &a); //TODO

//一些枚举量
enum class AugassignType{ //枚举augassign的类型
    equal,add,sub,mul,div,idiv,mod,none
};


#endif