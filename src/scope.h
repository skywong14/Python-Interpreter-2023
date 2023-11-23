//
// Created by skywa on 2023/11/23.
//
#pragma once
#include <iostream>
#include <vector>
#include <any>
#include <cstring>
#include "OPERATOR_ANY.h"
#ifndef PYTHON_INTERPRETER_SCOPE_H
#define PYTHON_INTERPRETER_SCOPE_H

struct NameSpace{
    std::map<std::string, std::any> Names;
    NameSpace(std::string str1,std::any a1){
        Names.clear();
        Names[str1]=a1;
    }
};
typedef std::vector<NameSpace>::iterator Scope_it;
typedef std::map<std::string, std::any>::iterator Names_it;
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


#endif //PYTHON_INTERPRETER_SCOPE_H
