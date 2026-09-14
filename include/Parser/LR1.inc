LR1(PT_Expr, KW_Return, PT_Constant)
LR1(PT_Stmt, PT_Expr, OP_Semicolon)

LR1(PT_BodyEnd, PT_Stmt, OP_BraceClose)
LR1(PT_BodyEnd, PT_Stmt, PT_BodyEnd)

LR1(PT_BodyEnd, PT_Stmts, OP_BraceClose)
LR1(PT_BodyEnd, PT_Stmts, PT_BodyEnd)

LR1(PT_Body, OP_BraceOpen, PT_BodyEnd)
LR1(PT_Body, OP_BraceOpen, OP_BraceClose)

LR1(PT_Args, OP_ParenOpen, OP_ParenClose)

LR1(PT_ArgsBody, PT_Args, PT_Body)
LR1(PT_FnBody, TT_Identifier, PT_ArgsBody)
LR1(PT_FnDefn, PT_Type, PT_FnBody)

LR1(PT_ArgsEnd, PT_Constant, OP_ParenClose)
LR1(PT_ArgsEnd, PT_Expr, OP_ParenClose)
LR1(PT_Args, OP_ParenOpen, PT_ArgsEnd)
LR1(PT_FnCall, TT_Identifier, PT_Args)
LR1(PT_Stmt, PT_FnCall, OP_Semicolon)
LR1(PT_FnDecl, PT_Type, PT_FnCall)
LR1(PT_Stmt, PT_FnDecl, OP_Semicolon)

LR1(PT_Bodies, PT_Body, PT_FnDefn)
LR1(PT_Bodies, PT_Bodies, PT_FnDefn)
LR1(PT_Bodies, PT_FnDefn, PT_Body)
LR1(PT_Bodies, PT_FnDefn, PT_Bodies)
LR1(PT_Bodies, PT_FnDefn, PT_FnDefn)

LR1(PT_IdStmt, TT_Identifier, OP_Semicolon)
LR1(PT_VarDecl, PT_Type, PT_IdStmt)
LR1(PT_RValue, PT_Binop, PT_Constant)
LR1(PT_RValue, PT_Binop, TT_Identifier)
LR1(PT_Expr, TT_Identifier, PT_RValue)

LR1(PT_Stmts, PT_Stmt, PT_Stmt)
LR1(PT_Stmts, PT_Stmts, PT_Stmt)
LR1(PT_Stmts, PT_Stmt, PT_Stmts)
LR1(PT_Stmts, PT_Stmts, PT_Stmts)

LR1(PT_VarDefn, PT_Type, PT_Expr)
LR1(PT_Stmt, PT_VarDefn, OP_Semicolon)
LR1(PT_Expr, PT_Constant, PT_RValue)
LR1(PT_RValue, PT_Binop, PT_Expr)
LR1(PT_Expr, PT_Expr, PT_RValue)