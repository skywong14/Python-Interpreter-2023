//
// Created by skywa on 2023/11/23.
//
//TODO 把变量传递改为常量引用
#pragma once
#include "Evalvisitor.h"
#ifndef PYTHON_INTERPRETER_SCOPE_H
#define PYTHON_INTERPRETER_SCOPE_H
#include <iostream>
#include <vector>
#include <map>
#include <any>
#include <cstring>
#include "OPERATOR_ANY.h"
//几个Built_in函数（似乎也可以被重载） int float string bool print
std::any builtin_Int_call(EvalVisitor &vis, Python3Parser::ArglistContext *ctx);
std::any builtin_Float_call(EvalVisitor &vis, Python3Parser::ArglistContext *ctx);
std::any builtin_Str_call(EvalVisitor &vis, Python3Parser::ArglistContext *ctx);
std::any builtin_Bool_call(EvalVisitor &vis, Python3Parser::ArglistContext *ctx);
std::any builtin_Print_call(EvalVisitor &vis, Python3Parser::ArglistContext *ctx);
//函数空间
typedef std::pair< std::vector<std::pair<std::string, std::any> >, Python3Parser::SuiteContext* > func_info;
struct func_NameSpace{
    std::map<std::string, func_info > Names;
    func_NameSpace(){
        Names.clear();
    }
};
typedef std::vector<func_NameSpace>::iterator func_Scope_it;
typedef std::map<std::string, std::any>::iterator Names_it;
typedef std::pair<func_Scope_it ,Names_it> Function_it;

//函数调用
void func_Define(std::string Name, std::vector<std::pair<std::string, std::any> > Arglists, Python3Parser::SuiteContext *ctx);

std::any func_call(std::string Name, EvalVisitor &vis, Python3Parser::ArglistContext *ctx);

void new_Funcspace();
void delete_Funcspace();


//--------------------------------------------

//变量空间
struct NameSpace{
    std::map<std::string, std::any> Names;
    NameSpace(){
        Names.clear();
    }
    NameSpace(std::string str1, std::any a1){
        Names.clear();
        Names[str1] = a1;
    }
    void set_New(std::string Name, std::any val){
//        Names.insert(std::make_pair(Name, val));
        Names[Name] = val;
    }
};

typedef std::vector<NameSpace>::iterator Scope_it;
//typedef std::map<std::string, std::any>::iterator Names_it; 已经定义
typedef std::pair<Scope_it ,Names_it> Variable_it;

Scope_it null_Scope();

Variable_it search_Built_in(std::string Name);
bool Variable_exist(std::string var_Name);
void set_Variable(std::string Name, std::any val);
//void newVariable(std::string var_Name, std::any value);//默认在栈顶scope内新建
Variable_it search_Scope(std::string var_Name);
Variable_it search_Scope(Scope_it it_Scope, std::string var_Name);
Variable_it new_Namespace(std::vector<std::pair<std::string, std::any> >);
void delete_Namespace();
std::any get_Value(std::string Name);
#endif //PYTHON_INTERPRETER_SCOPE_H
