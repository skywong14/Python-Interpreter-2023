//
// Created by sky on 2023/11/25.
//
#pragma once
#include "OPERATOR_ANY.h"
#include "scope.h"
#include <iostream>

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
    if (!ctx) return {};
    /*
    * TODO
    std::vector<Python3Parser::ArgumentContext*> Outputs = ctx->argument();
    if (!Outputs.empty()) {
        std::cout << Outputs.visit(Outputs[0]);
        for (int i = 1; i < list.size(); i++)
            std::cout << ' ' << Outputs.visit(Outputs[i]);
    }*/
    std::cout << std::endl;
    return {};
}