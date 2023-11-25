//
// Created by skywa on 2023/11/23.
//
#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <any>
#include <cstring>
#include "OPERATOR_ANY.h"
#include "Evalvisitor.h"
#ifndef PYTHON_INTERPRETER_SCOPE_H
#define PYTHON_INTERPRETER_SCOPE_H
//几个Built_in函数（似乎也可以被重载） int float string bool print
std::any builtin_Int_call(EvalVisitor &vis, Python3Parser::ArglistContext *ctx);
std::any builtin_Float_call(EvalVisitor &vis, Python3Parser::ArglistContext *ctx);
std::any builtin_Str_call(EvalVisitor &vis, Python3Parser::ArglistContext *ctx);
std::any builtin_Bool_call(EvalVisitor &vis, Python3Parser::ArglistContext *ctx);
std::any builtin_Print_call(EvalVisitor &vis, Python3Parser::ArglistContext *ctx);//TODO
//函数空间
struct func_NameSpace{
    //TODO
    std::map<std::string, std::any> Names;
    func_NameSpace(std::string str1, std::any a1){
        Names.clear();
        Names[str1]=a1;
    }
};
typedef std::vector<func_NameSpace>::iterator func_Scope_it;
typedef std::map<std::string, std::any>::iterator Names_it;
typedef std::pair<func_Scope_it ,Names_it> Function_it;

extern std::vector<func_NameSpace> func_scope;

//函数调用
std::any func_call(std::string Name, EvalVisitor &vis, Python3Parser::ArglistContext *ctx){
    auto func_scope_it = func_scope.end();
    func_scope_it--;
    while (func_scope_it != func_scope.begin()){
        auto find_it = (*func_scope_it).Names.find(Name);
        if ( find_it != (*func_scope_it).Names.end()) {
            return {};//TODO
        }
        func_scope_it--;
    }
    //在built_in中查找Name
    if (Name == "print"){
        return builtin_Print_call(vis, ctx);
    }else if (Name == "int"){
        return builtin_Int_call(vis, ctx);
    }else if (Name == "float"){
        return builtin_Float_call(vis, ctx);
    }else if (Name == "str"){
        return builtin_Str_call(vis, ctx);
    }else if (Name == "bool"){
        return builtin_Bool_call(vis, ctx);
    }else
        throw std::runtime_error("THE FUNCTION MIGHT NOT BE DEFINED");
}


//变量空间
struct NameSpace{
    std::map<std::string, std::any> Names;
    NameSpace(std::string str1, std::any a1){
        Names.clear();
        Names[str1]=a1;
    }
};

typedef std::vector<NameSpace>::iterator Scope_it;
//typedef std::map<std::string, std::any>::iterator Names_it; 已经定义
typedef std::pair<Scope_it ,Names_it> Variable_it;

extern std::vector<NameSpace> scope;

Variable_it search_Built_in(std::string var_Name){
    return {}; //TODO
}

Variable_it search_Scope(Scope_it it_Scope, std::string var_Name){
    if (it_Scope == scope.begin()){
        return search_Built_in(var_Name);
    }
    if (it_Scope == scope.end()) it_Scope--;

    auto ptr = (*it_Scope).Names.find(var_Name);
    if (ptr != (*it_Scope).Names.end()){//当前空间存在变量
        return std::make_pair(it_Scope, ptr);
    }else{
       it_Scope--;
       return search_Scope(it_Scope, var_Name);
    }
}
Variable_it newVariable_Scope(Scope_it it_Scope, std::string var_Name, std::any value){
    if (it_Scope == scope.end()) { //新建
        NameSpace new_space(var_Name, value);
        scope.push_back(new_space);
        auto it = scope.end();
        it--;
        return std::make_pair(it, (*it).Names.begin());
    }else{
        (*it_Scope).Names[var_Name] = value;
        return std::make_pair(it_Scope, (*it_Scope).Names.find(var_Name));
    }
}
std::any get_Value(std::string Name){
    Variable_it ptr = search_Scope( scope.end(), Name);
    return (ptr.second)->second;//返回any类型的变量值
}

#endif //PYTHON_INTERPRETER_SCOPE_H
