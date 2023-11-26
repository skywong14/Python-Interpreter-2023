//
// Created by sky on 2023/11/25.
//
#pragma once
#include "OPERATOR_ANY.h"
#include "scope.h"
#include "Evalvisitor.h"
#include <iostream>
//函数部分
func_NameSpace empty_funcspace;
std::vector<func_NameSpace> func_scope = {empty_funcspace};
std::any builtin_Int_call(EvalVisitor &vis, Python3Parser::ArglistContext *ctx){
    std::any val = vis.visitArglist(ctx);
    release_Tuple(val);
    return to_Int(val);
}
std::any builtin_Float_call(EvalVisitor &vis, Python3Parser::ArglistContext *ctx){
    std::any val = vis.visitArglist(ctx);
    release_Tuple(val);
    return to_Double(val);
}
std::any builtin_Str_call(EvalVisitor &vis, Python3Parser::ArglistContext *ctx){
    std::any val = vis.visitArglist(ctx);
    release_Tuple(val);
    return to_String(val);
}
std::any builtin_Bool_call(EvalVisitor &vis, Python3Parser::ArglistContext *ctx){
    std::any val = vis.visitArglist(ctx);
    release_Tuple(val);
    return to_Bool(val);
}
std::any builtin_Print_call(EvalVisitor &vis, Python3Parser::ArglistContext *ctx){
    Debug_output("__builtin_Print_call__");
    if (!ctx) return {};

    std::vector<Python3Parser::ArgumentContext*> Outputs = ctx->argument();
    if (!Outputs.empty()) {
        std::cout << vis.visitArgument(Outputs[0]);
        for (int i = 1; i < Outputs.size(); i++){
            std::cout << ' ' << vis.visitArgument(Outputs[i]);
        }
    }
    std::cout << std::endl;
    return {};
}

void func_Define(std::string Name, std::vector<std::pair<std::string, std::any> >Arglists, Python3Parser::SuiteContext *ctx){
    func_scope.back().Names[Name] = std::make_pair(Arglists, ctx);
}

//call分三步：创建变量和函数空间并初始化，执行suite，销毁空间
std::any func_call(std::string Name, EvalVisitor &vis, Python3Parser::ArglistContext *ctx){
    Debug_output("Function_call::"+Name);
    auto func_scope_it = func_scope.end();
    while (func_scope_it != func_scope.begin()){
        func_scope_it--;
        auto find_it = (*func_scope_it).Names.find(Name);
        if ( find_it != (*func_scope_it).Names.end()) {
            Debug_output("____find_the_function_____");
            std::vector<std::pair<std::string, std::any> > Arglists_init = (*find_it).second.first;

            if (ctx!=nullptr){
                std::vector<std::any> inits = std::any_cast<std::vector<std::any>>(vis.visitArglist(ctx)); //is a vector
                for (int i = 0; i < inits.size(); i++) {
                    Arglists_init[i].second = inits[i];
                }
            }

            new_Namespace(Arglists_init);
            new_Funcspace();


            Python3Parser::SuiteContext *func_ctx = (*find_it).second.second;
            std::any ret = vis.visitSuite(func_ctx);

            delete_Namespace();
            delete_Funcspace();

            return ret;
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

void new_Funcspace(){
    func_scope.push_back(empty_funcspace);
}
void delete_Funcspace(){
    func_scope.pop_back();
}

//----------------------------------

//变量部分
NameSpace empty_namespace;
std::vector<NameSpace> scope = {empty_namespace};

Scope_it null_Scope(){
    return scope.end();
}

//ATTENTION 函数内不能修改函数外的变量值,set local!!!
void set_Variable(std::string Name, std::any val){
    auto it = scope.end();
    it--;
    (*it).set_New(Name, val);
    /*Variable_it ptr = search_Scope(scope.end(), Name);
    if (ptr.first != scope.end()){
        (*(ptr.second)).second = val;
    }else{
        newVariable();
    }*/
}
bool Variable_exist(std::string var_Name){
    return (search_Scope(var_Name).first != null_Scope());
}
Variable_it search_Scope(std::string var_Name){
    return search_Scope(scope.end(), var_Name);
}
Variable_it search_Scope(Scope_it it_Scope, std::string var_Name){

    if (it_Scope == scope.end()) it_Scope--;

    auto ptr = (*it_Scope).Names.find(var_Name);
    if (ptr != (*it_Scope).Names.end()){//当前空间存在变量
        return std::make_pair(it_Scope, ptr);
    }else{
        if (it_Scope == scope.begin()){//未找到范围scope.end()
            return std::make_pair(scope.end(), empty_namespace.Names.end());
        }
        it_Scope--;
        return search_Scope(it_Scope, var_Name);
    }
}
Variable_it new_Namespace(std::vector<std::pair<std::string, std::any> >Arglists){
    NameSpace new_space;
    std::string Name;
    for (int i = 0; i < Arglists.size(); i++){
        Name = Arglists[i].first;
        new_space.Names[Name] = Arglists[i].second;
    }
    scope.push_back(new_space);
    auto it = scope.end();
    it--;
    return std::make_pair(it, (*it).Names.begin());
}
void delete_Namespace(){
    scope.pop_back();
}
std::any get_Value(std::string Name){
    Variable_it ptr = search_Scope( scope.end(), Name);
    return (ptr.second)->second;//返回any类型的变量值
}
