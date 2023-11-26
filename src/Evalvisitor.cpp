#pragma once
#include "Evalvisitor.h"
#include "OPERATOR_ANY.h"
#include "scope.h"



//函数部分
/*
funcdef: 'def' NAME parameters ':' suite;
parameters: '(' typedargslist? ')';
typedargslist: (tfpdef ('=' test)? (',' tfpdef ('=' test)?)*);
tfpdef: NAME ;
*/
std::any EvalVisitor::visitParameters(Python3Parser::ParametersContext *ctx){
    std::vector<std::pair<std::string, std::any> > Arglist_init;
    Arglist_init.clear();
    if (!ctx->typedargslist())
        return Arglist_init;
    auto types = ctx->typedargslist()->tfpdef();
    auto init_vals = ctx->typedargslist()->test();
    int no_val = types.size() - init_vals.size();
    for (int i = 0; i < types.size(); i++){
        std::string Variable_name = types[i]->NAME()->getText();
        std::any Variable_val = {};
        if (i >= no_val)
            Variable_val = visitTest(init_vals[i - no_val]);
        Arglist_init.emplace_back(std::make_pair(Variable_name, Variable_val));
    }
    return Arglist_init;
}
std::any EvalVisitor::visitFuncdef(Python3Parser::FuncdefContext *ctx){
    Debug_output("__FuncDefine__");
    std::string func_Name = ctx->NAME()->getText();
    //visitParameters返回vector
    std::vector<std::pair<std::string, std::any> > Arglists =
            std::any_cast<std::vector<std::pair<std::string, std::any> >>(visitParameters(ctx->parameters()));
    func_Define(func_Name, Arglists, ctx->suite());
    return {};
}

//三种FLOW-stmt
std::any EvalVisitor::visitFlow_stmt(Python3Parser::Flow_stmtContext *ctx){
    if (ctx->continue_stmt())
        return Flow_stmt::Flow_Continue;
    if (ctx->break_stmt())
        return Flow_stmt::Flow_Break;
    if (ctx->return_stmt()){
        std::vector<std::any> val = {};
        auto test_list = ctx->return_stmt()->testlist();
        if (test_list)
            val = std::any_cast<std::vector<std::any> >(visitTestlist(test_list));
        std::any val2 = val;
        return std::make_pair(Flow_stmt::Flow_Return, val2);
    }
    return visitChildren(ctx);
}
//IF WHILE

std::any EvalVisitor::visitIf_stmt(Python3Parser::If_stmtContext *ctx){
    Debug_output("If_stmt::");
    auto tests = ctx->test();
    auto suites = ctx->suite();
    //If && else if
    for (int i = 0; i < tests.size(); i++){
        std::any val = visitTest(tests[i]);
        release_Var(val);
        if (to_Bool(val))
            return visitSuite(suites[i]);
    }
    //else
    if (tests.size() < suites.size())
        return visitSuite(suites.back());
    return {};
}

antlrcpp::Any EvalVisitor::visitWhile_stmt(Python3Parser::While_stmtContext *ctx){
    Debug_output("While_stmt::");
    auto judge_test = ctx->test();
    auto suites = ctx->suite();
    std::any val_judge = visitTest(judge_test);
    release_Var(val_judge);
    while (to_Bool(val_judge)){
        Debug_output("_____In the while____");

        std::any execute = visitSuite(suites);

        if (is_FlowBreak(execute))
            break;
        if (is_FlowReturn(execute))
            return execute;

        val_judge = visitTest(judge_test);
        val_judge = visitTest(ctx->test());
        release_Var(val_judge);
    }
    return {};
}
//Expr:expr_stmt: testlist ( (augassign testlist) | ('=' testlist)*);
std::any EvalVisitor::visitExpr_stmt(Python3Parser::Expr_stmtContext *ctx){
    Debug_output("Expr_stmt");
    if (!(ctx->ASSIGN(0) || ctx->augassign())) return visitChildren(ctx);

    //先按等号分割
    auto ctx1 = ctx->testlist(); // ctx1:所有testlist,除最后一个外为左值
    int left_v_num = static_cast<int>(ctx1.size()) - 1;
    std::vector<std::any> right_v = std::any_cast<std::vector<std::any> >(visitTestlist(ctx1.back()));
    int right_size = right_v.size();
    for (int i = 0; i < right_size; i++)
        release_Var(right_v[i]);
    if (right_size == 1 && std::any_cast<std::vector<std::any> >(&right_v[0])){
        std::vector<std::any> expand_r = std::any_cast<std::vector<std::any> >(right_v[0]);
        right_v = expand_r;
    }
    std::any right_v_all = right_v;
    release_Tuple(right_v_all);
    //ATTENTION: visitTestlist返回一个vector！！！(release后如果是单个元素，加上一层）

    std::string Variable_name;
    if (ctx->augassign()){
        //augassign赋值 ATTENTION:tuple不合法，不用管
        std::string opt = ctx->augassign()->getText();
        Variable_name = ctx1[0]->getText() ;
        //如果未定义应当报错
        if (!Variable_exist(Variable_name)) //需要考虑是不是当前namespace空间?
            throw std::runtime_error("Have not define Augassign left value");
        Variable_it ptr = search_Scope(Variable_name);
        std::any val =  (ptr.second)->second;
        std::any sol;
        if (opt == "+="){
            sol = val + right_v_all;
        } else if (opt == "-="){
            sol = val - right_v_all;
        } else if (opt == "*="){
            sol = val * right_v_all;
        } else if (opt == "/="){
            sol = val / right_v_all;
        } else if (opt == "//="){
            if (is_Integer(val) && is_Integer(right_v_all))
                sol = DivInt(val, right_v_all);
            else
                sol = DivDouble(val, right_v_all);
        } else if (opt == "%="){
            sol = val % right_v_all;
        }
        set_Variable(Variable_name, sol);
        return sol;
    }else{
        //等号赋值
        for (int i = 0; i < left_v_num; i++){
            std::vector<std::any> left_v = std::any_cast<std::vector<std::any> >(visitTestlist(ctx1[i]));
            if (left_v.size() == 1){
                Variable_name = get_varName(left_v[0]);
                Debug_output("____LEFT_V==1___NAME::"+Variable_name);
                set_Variable(Variable_name, right_v_all);
            }else{
                Debug_output("____LEFT_V==TUPLE____");
                for (int j = 0; j < left_v.size(); j++){
                    Variable_name = get_varName(left_v[j]);
                    set_Variable(Variable_name, right_v[j]);
                }
            }
        }
        return {};
    }
    return {};
}
//stmt操作
std::any EvalVisitor::visitSimple_stmt(Python3Parser::Simple_stmtContext *ctx){
//    Debug_output("Simple_stmt");
    return visitSmall_stmt(ctx->small_stmt());
}
std::any EvalVisitor::visitSmall_stmt(Python3Parser::Small_stmtContext *ctx){
//    Debug_output("Small_stmt");
    if (ctx->flow_stmt()){
        return visitFlow_stmt(ctx->flow_stmt());
    }
    return visitExpr_stmt(ctx->expr_stmt());
}
//suite
std::any EvalVisitor::visitSuite(Python3Parser::SuiteContext *ctx){
    Debug_output("Suite_stmt");
    //简单语句
    if (ctx->simple_stmt()){
        return visitSimple_stmt(ctx->simple_stmt());
    }
    //复合语句：依次执行，除非有flow控制
    for (int i = 0; ctx->stmt(i); i++){
        std::any st1 = visitStmt(ctx->stmt(i));
        if (is_FlowBreak(st1))
            return Flow_stmt::Flow_Break;
        if (is_FlowContinue(st1))
            return Flow_stmt::Flow_Continue;
        if (is_FlowReturn(st1)){
            std::any val = (std::any_cast<std::pair<Flow_stmt, std::any> >(st1)).second;
            release_Tuple(val);
            return val;
        }
    }
    return {};
}

//Or_test && And_test
std::any EvalVisitor::visitOr_test(Python3Parser::Or_testContext *ctx){
    if (ctx->OR(0)){
//        Debug_output("Or_test");
        std::vector<Python3Parser::And_testContext *> andtext = ctx->and_test();
        for (auto it = andtext.begin(); it != andtext.end(); it++){
            std::any val = visitAnd_test((*it));
            release_Var(val);
            release_Tuple(val);
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
//        Debug_output("And_test");
        std::vector<Python3Parser::Not_testContext *> nottext = ctx->not_test();
        for (auto it = nottext.begin(); it != nottext.end(); it++){
            std::any val = visitNot_test((*it));
            release_Var(val);
            release_Tuple(val);
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
//        Debug_output("Not_test");
        std::any val = visitNot_test(ctx->not_test()); //TLE:可以&&改为传输右值
        release_Var(val);
        release_Tuple(val);
        return !to_Bool(val);
    }else{
        return visitComparison(ctx->comparison());
    }
}
//比较
std::any EvalVisitor::visitComparison(Python3Parser::ComparisonContext *ctx){
    if (ctx->comp_op(0)){
        Debug_output("Comparison");
        auto ctx1 = ctx->arith_expr();
        auto ctx2 = ctx->comp_op();

        std::any left_v = visitArith_expr(ctx1[0]),right_v;
        release_Tuple(left_v);
        release_Var(left_v);
        bool flag=true;
        for (int i = 0; i < ctx2.size(); i++){
            right_v = visitArith_expr(ctx1[i + 1]);
            release_Tuple(right_v);
            release_Var(right_v);
            auto *const op1 = ctx2[i]; //QUESTION:why *const?
            if (op1->EQUALS())
                flag = (left_v == right_v);
            else if (op1->LESS_THAN())
                flag = (left_v < right_v);
            else if (op1->GREATER_THAN())
                flag = (left_v > right_v);
            else  if (op1->GT_EQ())
                flag = (left_v >= right_v);
            else if (op1->LT_EQ())
                flag = (left_v <= right_v);
            else if (op1->NOT_EQ_2())
                flag = (left_v != right_v);

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
    Debug_output("Arith_expr");
    release_Var(now_v);
    release_Tuple(now_v);
    for (int i = 0; i < ctx2.size(); i++){
        tmp = visitTerm(ctx1[i+1]);
        release_Var(tmp);
        if (ctx2[i]->ADD())
            now_v += tmp;
        else if (ctx2[i]->MINUS())
            now_v -= tmp;
    }
    return now_v;
}

std::any EvalVisitor::visitTerm(Python3Parser::TermContext *ctx){
    Debug_output("Term");
    auto ctx1 = ctx->factor();
    auto ctx2 = ctx->muldivmod_op();
    std::any now_v = visitFactor(ctx1[0]), tmp; //ATTENTION:如果ctx1[0]的值不存在？
    if (ctx2.empty()) return now_v;
    release_Var(now_v);
    release_Tuple(now_v);
    for (int i = 0; i < ctx2.size(); i++){
        tmp = visitFactor(ctx1[i+1]);
        release_Tuple(tmp);//update
        release_Var(tmp);
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
std::any EvalVisitor::visitAugassign(Python3Parser::AugassignContext *ctx){
    Debug_output("Augassign");
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

//Factor 正负号
std::any EvalVisitor::visitFactor(Python3Parser::FactorContext *ctx){
    if (ctx->atom_expr())
        return visitAtom_expr(ctx->atom_expr());
    Debug_output("Factor");
    std::any inside = visitFactor(ctx->factor());
    release_Var(inside);
    release_Tuple(inside);
    if (ctx->MINUS()){
        inside = -inside;
    }
    return inside;
}

//atom 和 atom expr
std::any EvalVisitor::visitAtom_expr(Python3Parser::Atom_exprContext *ctx){
    if (ctx->trailer()){ //有括号，是函数
        Debug_output("Atom_expr::expr-FunctionName");
        std::any val_atom = visitAtom(ctx->atom());
        release_Var(val_atom);
        std::string Name = to_String( val_atom );
        return func_call(Name, *this, ctx->trailer()->arglist()); //return vector?
    }else{
        Debug_output("Atom_expr::atom");
        std::any val = visitAtom(ctx->atom());
        return val;
    }
}

std::any EvalVisitor::visitAtom(Python3Parser::AtomContext *ctx){
    if (ctx->NAME()){
        //如果不在变量lst内，就返回原来内容
        std::string str = ctx->NAME()->getText();
        Debug_output("Atom-NAME::"+str);
        if (search_Scope(str).first != null_Scope() ){
            return std::make_pair(str, get_Value( str )); //函数名+函数值
        }else{
            return str;
        }
    }
    Debug_output("Atom-Not NAME");
    if (ctx->NUMBER()){
        std::string str = ctx->getText();
        Debug_output("Atom:"+str);
        if (str.find('.') == std::string::npos){
            return String_to_Int(str); //to Int
        }
        else
            return std::stod(str); //to Double
    }
    else if (ctx->TRUE())
        return true;
    else if (ctx->FALSE())
        return false;
    else if (ctx->NONE())
        return {};
    else if (ctx->test())
        return visitTest(ctx->test());
    else {
        auto str_list = ctx->STRING();
        std::string str = str_list[0]->getText();
        str = str.substr(1, str.size() - 2);
        return str;
    }
}
//TEST

//Testlist 必须是一个vector
std::any EvalVisitor::visitTestlist(Python3Parser::TestlistContext *ctx){
    std::vector<std::any> vars;
    vars.clear();
    for (int i = 0; auto ctx2 = ctx->test(i); i++){
        std::any res = visitTest(ctx2);
        //Tuple或单个元素
        if (is_Tuple(res)){
            std::vector<std::any> vars2 = std::any_cast<std::vector<std::any>>(res);
            for (int i = 0 ; i < vars2.size(); i++)
                vars.emplace_back( vars2[i] );
            //TLE:使用vector的insert?
        }else{
            vars.emplace_back(res); //TLE:改为move？
        }
    }
    return vars;
}

std::any EvalVisitor::visitArglist(Python3Parser::ArglistContext *ctx){
    auto ctx1 = ctx->argument();
    std::vector<std::any> ret_array;
    ret_array.clear();
    for (int i = 0; i < ctx1.size(); i++){
        std::any val = visitArgument(ctx1[i]);
        ret_array.push_back(val);
    }
    return ret_array;
}