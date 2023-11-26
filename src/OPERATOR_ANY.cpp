// Created by skywa on 2023/11/19.
#pragma once
#include <any>
#include <iomanip>
#include "OPERATOR_ANY.h"
//#include "Bigint.h"
#include <vector>
#define Int sjtu::int2048
//For debug only
void Debug_output(std::string s){
//    std::cout<<s<<std::endl;
}

double Int_to_Double(Int x){
    return x.int2048_to_Double();
}
Int String_to_Int(std::string x){
    sjtu::int2048 ans(x);
    return ans;
}

//判断类型

bool is_Int(std::any const &a){
    return (std::any_cast<Int>(&a) != nullptr);
}
bool is_Double(std::any const &a){
    return (std::any_cast<double>(&a) != nullptr);
}
bool is_Bool(std::any const &a){
    return (std::any_cast<bool>(&a) != nullptr);
}
bool is_String(std::any const &a){
    return (std::any_cast<std::string>(&a) != nullptr);
}
bool is_Integer(std::any const &a) {
    return is_Int(a) || is_Bool(a);
}
bool is_Number(std::any const &a) {
    return is_Int(a) || is_Double(a) || is_Bool(a);
}
bool is_Tuple(std::any const &a){
    return (std::any_cast<std::vector<std::any> >(&a));
}
bool is_None(std::any const &a){
    return !a.has_value();
}
bool is_FlowBreak(std::any const &a){
    auto ptr = std::any_cast<Flow_stmt>(&a);
    if (ptr != nullptr){
        return (std::any_cast<Flow_stmt>(a) == Flow_stmt::Flow_Break);
    }
    return false;
}
bool is_FlowContinue(std::any const &a){
    auto ptr = std::any_cast<Flow_stmt>(&a);
    if (ptr != nullptr){
        return (std::any_cast<Flow_stmt>(a) == Flow_stmt::Flow_Continue);
    }
    return false;
}
bool is_FlowReturn(std::any const &a){
    if (std::any_cast<std::pair<Flow_stmt, std::any> >(&a) != nullptr){
        std::pair<Flow_stmt, std::any> tmp = std::any_cast<std::pair<Flow_stmt, std::any> >(a);
        return (tmp.first == Flow_stmt::Flow_Return);
    }
    return false;
}
//关于变量
bool is_Var(std::any const &a){
    return (std::any_cast<std::pair<std::string, std::any>>(&a));
}

void release_Tuple(std::any &a){ //把(var)释放成var
    //ATTENTION:()也是Tuple，而不是None
    std::vector<std::any>* ptr1=std::any_cast<std::vector<std::any> >(&a);
    if (!ptr1) return;//Tuple()
//    Debug_output("IS TUPLE");
    if (ptr1->size() == 1) {
        a = (*ptr1)[0];
        release_Tuple(a);
    }else if (ptr1->size() > 0){
            for (int i = 0; i < ptr1->size(); i++)//ATTENTION:这里用了小int
                release_Tuple((*ptr1)[i]);
    }
}//PERHAPS WRONG!!!!!

//类型转换
Int to_Int(std::any const &a){
    if (is_Int(a)){
        Int val = std::any_cast<Int>(a);
        return val;
    }
    if (is_Bool(a)) {
        if (std::any_cast<bool>(a)) return 1;
        return 0;
    }
    if (is_Double(a)){
        //ATTENEION:python中取整方式是否不同？
        long long tmp = (long long)std::any_cast<double>(a);
        Int tmp2(tmp);
        return tmp2;
    }
    throw std::runtime_error("Invalid type at to_Int");
//    return 0;
}
double to_Double(std::any const &a){
    if (is_Double(a)) return (std::any_cast<double>(a));
    if (is_Bool(a)) {
        if (std::any_cast<bool>(a)) return 1;
        return 0;
    }
    if (is_Int(a)){
        Int tmp = (Int)std::any_cast<Int>(a);
        return Int_to_Double(tmp);
    }
    throw std::runtime_error("Invalid type at to_Double");
    return 0;
}
std::string to_String(std::any const &a){
    if (is_String(a)) return (std::string)std::any_cast<std::string>(a);
    throw std::runtime_error("Invalid type at to_String");
}
bool to_Bool(std::any const &a){
    if (is_Bool(a)) return (std::any_cast<bool>(a));
    if (is_Int(a)){
        if (std::any_cast<Int>(a)!=0) return true;
        else return false;
    }
    if (is_Double(a)) {
        if (fabs(std::any_cast<double>(a))<1e-9) return false;
        return true;
    }
    if (is_String(a)) {
        std::string tmp = to_String(a);
        if (tmp.length() > 0) return true;
        return false;
    }
    //TODO None and Tuple待完成
    throw std::runtime_error("Invalid type at to_Bool");
}
void release_Var(std::any &a){
    if (std::any_cast<std::vector<std::any>>(&a)) {
        std::vector<std::any> tmp = std::any_cast<std::vector<std::any>>(a);
        for (int i = 0; i < tmp.size(); i++)
            release_Var( tmp[i] );
        a = tmp;
    }
    if (is_Var(a))
        a = std::any_cast<std::pair<std::string, std::any>>(a).second;
}
std::string get_varName(std::any const &a){
    if (is_Var(a))
        return std::any_cast<std::pair<std::string, std::any>>(a).first;
    return to_String(a);
}

//四则运算
std::any operator+(std::any const &a1, std::any const &a2){
    std::any a3;
    //Int Double Bool运算时返回数字
    if (is_Number(a1) && is_Number(a2)){
        if (is_Double(a1) || is_Double(a2)){
            double tmp = to_Double(a1) + to_Double(a2);
            a3 = tmp;
            return a3;
        }else{
            Int tmp = to_Int(a1) + to_Int(a2);
            a3=tmp;
            return a3;
        }
    }
    if (is_String(a1) && is_String(a2)){
        std::string tmp = to_String(a1) + to_String(a2);
        a3 = tmp;
        return a3;
    }
    if (is_Tuple(a1) && is_Tuple(a2)){
        //待完成
    }
    //其他情况
    throw std::runtime_error("Undefined at operator+");
//    return 0;
}
std::any operator-(std::any const &any1, std::any const &any2){
    std::any a1=any1, a2=any2;
    release_Var(a1); release_Var(a2);
    if (is_Number(a1) && is_Number(a2)){
        if (is_Double(a1) || is_Double(a2)){
            double tmp = to_Double(a1) - to_Double(a2);
            return tmp;
        }else {
            Int tmp = to_Int(a1) - to_Int(a2);
            return tmp;
        }
    }
    throw std::runtime_error("Undefined at operator-");
}
std::any operator*(std::any const &any1, std::any const &any2){
    std::any a1=any1, a2=any2;
    release_Var(a1); release_Var(a2);
    if (is_Number(a1) && is_Number(a2)) {
        if (is_Double(a1) || is_Double(a2)){
            double tmp = to_Double(a1) * to_Double(a2);
            return tmp;
        }else {
            Int tmp = to_Int(a1) * to_Int(a2);
            return tmp;
        }
    }
    if (is_Integer(a1) && is_String(a2)){
        Int tmp = to_Int(a1);
        std::string s1 = "", s2 = to_String(a2); //Question:why warning?
        for (int i = 1; i <= tmp; i++) s1 += s2;
        return s1;
    }
    if (is_Integer(a2) && is_String(a1)){
        Int tmp = to_Int(a2);
        std::string s1 = "",s2 = to_String(a1); //Question:why warning?
        for (int i = 1; i <= tmp; i++) s1 += s2;
        return s1;
    }//TLE
//    std::cout<<a1<<','<<a2<<std::endl;
    throw std::runtime_error("Undefined at operator*");
}

std::any operator/(std::any const &a1, std::any const &a2){
    if (is_Number(a1) && is_Number(a2)){
        double p = to_Double(a1) , q = to_Double(a2);
        if (fabs(q)<(1e-10)) throw std::runtime_error("DIV0 at operator/");
        p = p / q;
        return p;
    }
    throw std::runtime_error("Undefined at operator/");
}

std::any operator-(std::any const &a1) {
    if (is_Double(a1))
        return -to_Double(a1);
    else if (is_Integer(a1))
        return -to_Int(a1);
    throw std::runtime_error("Undefined at operator set_positive");
}

//div & mod
std::any DivInt(std::any const &any1, std::any const &any2){
    std::any a1=any1, a2=any2;
    release_Var(a1); release_Var(a2);
    if (is_Number(a1) && is_Number(a2)){
        if (is_Double(a1) || is_Double(a2)){
            double n1 = to_Double(a1),n2 = to_Double(a2);
            return (Int)(floor(n1 / n2));
        }
        Int n1 = to_Int(a1),n2 = to_Int(a2);
        return (Int)(n1 / n2);
    }
    throw std::runtime_error("Undefined at operator DivInt");
}
std::any DivDouble(std::any const &a1, std::any const &a2){
    if (is_Number(a1) && is_Number(a2)){
        double n1 = to_Double(a1), n2 = to_Double(a2);
        return n1 / n2;
    }
    throw std::runtime_error("Undefined at operator DivDouble");
}
std::any operator%(std::any const &a1, std::any const &a2){
    return a1 - DivInt(a1, a2) * a2;
}
//Augassign
std::any &operator+=(std::any &a1, std::any const &a2){
    return a1 = a1 + a2;
}

std::any &operator-=(std::any &a1, std::any const &a2){
    return a1 = a1 - a2;
}

std::any &operator*=(std::any &a1, std::any const &a2){
    return a1 = a1 * a2;
}

std::any &operator%=(std::any &a1, std::any const &a2){
    a1 -= DivInt(a1, a2) * a2;
    return a1;
}

//位运算 Question:需要实现吗

//逻辑运算 not:返回bool  and/or:

// 比较运算符
bool operator==(std::any const &a1, std::any const &a2){
    //不同类型？
    if (is_None(a1) || is_None(a2)) return is_None(a1) && is_None(a2);
    if (is_Number(a1) && is_Number(a2)){
        double n1 = to_Double(a1), n2 = to_Double(a2);
        if (fabs(n1 - n2) < 1e-10) return true;
        return false;
    }
    if (is_String(a1) && is_String(a2)){
        return to_String(a1) == to_String(a2);
    }
    return false;
}
bool operator<(std::any const &a1, std::any const &a2){
    if (is_Double(a1) && is_Double(a2)){
        return to_Double(a1) < to_Double(a2);
    }
    if (is_Integer(a1) && is_Integer(a2)){
        return to_Int(a1) < to_Int(a2);
    }
    if (is_String(a1) && is_String(a2)){
        return to_String(a1) < to_String(a2);
    }
    throw std::runtime_error("Undefined at operator<");
}
bool operator>(std::any const &a1, std::any const &a2){
    return (a2 < a1);
}

bool operator>=(std::any const &a1, std::any const &a2){
    return !(a1 < a2);
}

bool operator<=(std::any const &a1, std::any const &a2){
    return !(a2 < a1);
}

bool operator!=(std::any const &a1, std::any const &a2){
    return !(a1 == a2);
}
//AugAssign
std::ostream &operator<<(std::ostream &os, std::any const &a){
    if (is_None(a)) os << "None";
    else if (is_Bool(a)) os << (to_Bool(a) ? "True" : "False");
    else if (is_Int(a)) os << to_Int(a);
    else if (is_Double(a)){
        double tmp = to_Double(a);
        if (fabs(tmp) < 1e-9) tmp=0;
        os << std::fixed << std::setprecision(6) << tmp; //学自Wkp
    } else if (is_Var(a)){
        std::any b=a;
        release_Var(b);
        release_Tuple(b);
        os << b;
    } else if (is_String(a)){
        os << to_String(a);
    } else if (is_Tuple(a)){

    }
    return os;
}


