//
// Created by sky on 2023/11/25.
//
#pragma once
#include "OPERATOR_ANY.h"
#include "scope.h"
#include <iostream>
//函数部分
std::vector<func_NameSpace> func_scope;
std::any builtin_Int_call(EvalVisitor &vis, Python3Parser::ArglistContext *ctx){
    return to_Int(vis.visit(ctx));
}
std::any builtin_Float_call(EvalVisitor &vis, Python3Parser::ArglistContext *ctx){
    return to_Double(vis.visit(ctx));
}
std::any builtin_Str_call(EvalVisitor &vis, Python3Parser::ArglistContext *ctx){
    return to_String(vis.visit(ctx));
}
std::any builtin_Bool_call(EvalVisitor &vis, Python3Parser::ArglistContext *ctx){
    return to_Bool(vis.visit(ctx));
}
std::any builtin_Print_call(EvalVisitor &vis, Python3Parser::ArglistContext *ctx){
    Debug_output("__builtin_Print_call__");
    if (!ctx) return {};

    std::vector<Python3Parser::ArgumentContext*> Outputs = ctx->argument();
    if (!Outputs.empty()) {
        std::cout << vis.visit(Outputs[0]);
        for (int i = 1; i < Outputs.size(); i++)
            std::cout << ' ' << vis.visit(Outputs[i]);
    }
    std::cout << std::endl;
    return {};
}

Variable_it search_Built_in(std::string Name){
    //TODO
}

std::any func_call(std::string Name, EvalVisitor &vis, Python3Parser::ArglistContext *ctx){
    Debug_output("Function_call::"+Name);
    auto func_scope_it = func_scope.end();
    while (func_scope_it != func_scope.begin()){
        func_scope_it--;
        auto find_it = (*func_scope_it).Names.find(Name);
        if ( find_it != (*func_scope_it).Names.end()) {
            return {};//TODO
        }
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



//变量部分
NameSpace empty_namespace;
std::vector<NameSpace> scope = {};

Scope_it null_Scope(){
    return scope.end();
}

//ATTENTION 函数内不能修改函数外的变量值,set local!!!
void set_Variable(std::string Name, std::any val){
    auto it = scope.end();
    it--;
    (*it).Names[Name]=val;
    /*Variable_it ptr = search_Scope(scope.end(), Name);
    if (ptr.first != scope.end()){
        (*(ptr.second)).second = val;
    }else{
        newVariable();
    }*/
}

Variable_it search_Scope(std::string var_Name){
    return search_Scope(scope.end(), var_Name);
}
Variable_it search_Scope(Scope_it it_Scope, std::string var_Name){
    if (it_Scope == scope.begin()){//未找到范围scope.end()
        return std::make_pair(scope.end(), empty_namespace.Names.end());
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
Variable_it new_Namespace(std::string var_Name, std::any value){
    //TODO
    NameSpace new_space(var_Name, value);
    scope.push_back(new_space);
    auto it = scope.end();
    it--;
    return std::make_pair(it, (*it).Names.begin());
}
std::any get_Value(std::string Name){
    Variable_it ptr = search_Scope( scope.end(), Name);
    return (ptr.second)->second;//返回any类型的变量值
}
