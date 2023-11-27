#pragma once
#ifndef PYTHON_INTERPRETER_EVALVISITOR_H
#define PYTHON_INTERPRETER_EVALVISITOR_H


#include "Python3ParserBaseVisitor.h"


class EvalVisitor : public Python3ParserBaseVisitor {
    // TODO: override all methods of Python3ParserBaseVisitor
public:
//    virtual std::any visitFile_input(Python3Parser::File_inputContext *ctx) override;

    virtual std::any visitFuncdef(Python3Parser::FuncdefContext *ctx) override;

    virtual std::any visitParameters(Python3Parser::ParametersContext *ctx) override;

//    virtual std::any visitStmt(Python3Parser::StmtContext *ctx) override;

    virtual std::any visitSimple_stmt(Python3Parser::Simple_stmtContext *ctx) override;

    virtual std::any visitSmall_stmt(Python3Parser::Small_stmtContext *ctx) override;

    virtual std::any visitAugassign(Python3Parser::AugassignContext *ctx) override;

    virtual std::any visitExpr_stmt(Python3Parser::Expr_stmtContext *ctx) override;

    virtual std::any visitFlow_stmt(Python3Parser::Flow_stmtContext *ctx) override;

//    virtual std::any visitCompound_stmt(Python3Parser::Compound_stmtContext *ctx) override;

    virtual std::any visitIf_stmt(Python3Parser::If_stmtContext *ctx) override;

    virtual std::any visitWhile_stmt(Python3Parser::While_stmtContext *ctx) override;

    virtual std::any visitSuite(Python3Parser::SuiteContext *ctx) override;

//    virtual std::any visitTest(Python3Parser::TestContext *ctx) override;

    virtual std::any visitOr_test(Python3Parser::Or_testContext *ctx) override;

    virtual std::any visitAnd_test(Python3Parser::And_testContext *ctx) override;

    virtual std::any visitNot_test(Python3Parser::Not_testContext *ctx) override;

    virtual std::any visitComparison(Python3Parser::ComparisonContext *ctx) override;

    virtual std::any visitArith_expr(Python3Parser::Arith_exprContext *ctx) override;

    virtual std::any visitTerm(Python3Parser::TermContext *ctx) override;

    virtual std::any visitFactor(Python3Parser::FactorContext *ctx) override;

    virtual std::any visitAtom_expr(Python3Parser::Atom_exprContext *ctx) override;

//    virtual std::any visitTrailer(Python3Parser::TrailerContext *ctx) override;

    virtual std::any visitAtom(Python3Parser::AtomContext *ctx) override;

    virtual std::any visitTestlist(Python3Parser::TestlistContext *ctx) override;

    virtual std::any visitArglist(Python3Parser::ArglistContext *ctx) override;

    virtual std::any visitArgument(Python3Parser::ArgumentContext *ctx) override;
};

#endif//PYTHON_INTERPRETER_EVALVISITOR_H
