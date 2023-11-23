#pragma once
#include "Evalvisitor.h"
#include "OPERATOR_ANY.h"
#include "SCOPE.h"


//函数部分
/*
 file_input: (NEWLINE | stmt)* EOF;
funcdef: 'def' NAME parameters ':' suite;
parameters: '(' typedargslist? ')';
typedargslist: (tfpdef ('=' test)? (',' tfpdef ('=' test)?)*);
tfpdef: NAME ;
 */





//三种FLOW-stmt

//IF WHILE





//基本操作
//Test

//Or_test  && And_test
std::any EvalVisitor::visitOr_test(Python3Parser::Or_testContext *ctx){
    if (ctx->OR(0)){
        std::vector<Python3Parser::And_testContext *> andtext = ctx->and_test();
        for (auto it = andtext.begin(); it != andtext.end(); it++) {
            std::any val = visitAnd_test((*it));
            release_Var(val);
            if (to_Bool(val))
                return true;
        }
        return false;
    }else{
       // return antlr4::tree::AbstractParseTreeVisitor::visitChildren(ctx);
        return visitChildren(ctx);
    }
}
std::any EvalVisitor::visitAnd_test(Python3Parser::And_testContext *ctx){
    if (ctx->AND(0)){
        std::vector<Python3Parser::Not_testContext *> nottext = ctx->not_test();
        for (auto it = nottext.begin(); it != nottext.end(); it++) {
            std::any val = visitNot_test((*it));
            release_Var(val);
            if (!to_Bool(val))
                return false;
        }
        return true;
    }else{
        // return antlr4::tree::AbstractParseTreeVisitor::visitChildren(ctx);
        return visitChildren(ctx);
    }
}
std::any EvalVisitor::visitNot_test(Python3Parser::Not_testContext *ctx){
    if (ctx->NOT()){
        std::any val = visitNot_test(ctx->not_test()); //TLE:可以&&改为传输右值
        release_Var(val);
        return !to_Bool(val);
    }else{
        return visitComparison(ctx->comparison());
    }
}
//比较
std::any EvalVisitor::visitComparison(Python3Parser::ComparisonContext *ctx) {
    if (ctx->comp_op(0)) {
        auto ctx1 = ctx->arith_expr();
        auto ctx2 = ctx->comp_op();

        std::any left_v = visitArith_expr(ctx1[0]),right_v;
        bool flag=true;
        for (int i = 0; i < ctx2.size(); i++){
            right_v = visitArith_expr(ctx1[i + 1]);
            
            auto *const op1 = ctx2[i]; //QUESTION:why *const?
            if (op1->EQUALS())
                flag = left_v == right_v;
            else if (op1->LESS_THAN())
                flag = left_v < right_v;
            else if (op1->GREATER_THAN())
                flag = left_v > right_v;
            else  if (op1->GT_EQ())
                flag = left_v >= right_v;
            else if (op1->LT_EQ())
                flag = left_v <= right_v;
            else if (op1->NOT_EQ_2())
                flag = left_v != right_v;

            if (!flag) return false;
            left_v = right_v; //TLE: Move()?
        }
        return true;
    }else{
        return visitChildren(ctx);
    }
}
//算术运算
std::any EvalVisitor::visitArith_expr(Python3Parser::Arith_exprContext *ctx){
    auto ctx1 = ctx->term();
    auto ctx2 = ctx->addorsub_op();
    std::any now_v = visitTerm(ctx1[0]), tmp;
    if (ctx2.empty()) return now_v;
    release_Var(now_v);
    for (int i = 0; i < ctx2.size(); i++){
        tmp = visitTerm(ctx1[i+1]);
        if (ctx2[i]->ADD())
            now_v += tmp;
        else if (ctx2[i]->MINUS())
            now_v -= tmp;
    }
}

std::any EvalVisitor::visitTerm(Python3Parser::TermContext *ctx){
    auto ctx1 = ctx->factor();
    auto ctx2 = ctx->muldivmod_op();
    std::any now_v = visitFactor(ctx1[0]), tmp; //ATTENTION:如果ctx1[0]的值不存在？
    if (ctx2.empty()) return now_v;
    release_Var(now_v);
    for (int i = 0; i < ctx2.size(); i++){
        tmp = visitFactor(ctx1[i+1]);
        //QUESTION:会返回(VarType)吗？
        if (ctx2[i]->STAR())
            now_v *= tmp;
        else if (ctx2[i]->DIV())
            now_v = DivDouble(now_v, tmp);
        else if (ctx2[i]->IDIV())
            now_v = DivInt(now_v, tmp);
        else if (ctx2[i]->MOD())
            now_v %= tmp;
    }
    return now_v;
}

//函数操作
std::any EvalVisitor::visitAugassign(Python3Parser::AugassignContext *ctx) {
    if (ctx->ADD_ASSIGN())
        return AugassignType::add;
    else if (ctx->SUB_ASSIGN())
        return AugassignType::sub;
    else if (ctx->MULT_ASSIGN())
        return AugassignType::mul;
    else if (ctx->DIV_ASSIGN())
        return AugassignType::div;
    else if (ctx->IDIV_ASSIGN())
        return AugassignType::idiv;
    else if (ctx->MOD_ASSIGN())
        return AugassignType::mod;
    else
        return AugassignType::none;
}

std::any EvalVisitor::visitExpr_stmt(Python3Parser::Expr_stmtContext *ctx){
    if (!(ctx->ASSIGN(0) || ctx->augassign())) return visitChildren(ctx);
    //先按等号分割
    auto ctx1 = ctx->testlist(); // ctx1:所有testlist
    int left_v_num = static_cast<int>(ctx1.size()) - 1;
    std::vector<std::any> right_v = std::any_cast<std::vector<std::any> >(visitTestlist(ctx1.back()));
    //ATTENTION: visitTestlist返回一个vector！！！(release后如果是单个元素，加上一层）
    int right_size = right_v.size();
    //right_size = 1 or right_size > 1
    for (int i = 0; i < left_v_num; i++){
        std::vector<std::any> left_v = std::any_cast<std::vector<std::any> >(visitTestlist(ctx1.back()));
        if (left_v.size() == 1){

        }
    }

}