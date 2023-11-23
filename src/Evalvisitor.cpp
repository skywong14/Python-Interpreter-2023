#include "Evalvisitor.h"
#include "OPERATOR_ANY.h"


//函数部分
/*
 file_input: (NEWLINE | stmt)* EOF;
funcdef: 'def' NAME parameters ':' suite;
parameters: '(' typedargslist? ')';
typedargslist: (tfpdef ('=' test)? (',' tfpdef ('=' test)?)*);
tfpdef: NAME ;
 */


//三种FLOWstmt

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