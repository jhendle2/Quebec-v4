LR0(PT_Type, KW_Int)
LR0(PT_Type, KW_Void)
LR0(PT_Type, KW_Float)
LR0(PT_Type, KW_Char)

LR0(PT_Constant, TT_ConstInt)
LR0(PT_Constant, TT_ConstOct)
LR0(PT_Constant, TT_ConstHex)
LR0(PT_Constant, TT_ConstFloat)
LR0(PT_Constant, TT_ConstChar)
LR0(PT_Constant, TT_ConstString)

// LR0(PT_Body, PT_FnDefn)
LR0(PT_Stmt, PT_VarDecl)

LR0(PT_Binop, OP_Assign)
LR0(PT_Binop, OP_Plus)
LR0(PT_Binop, OP_Minus)
LR0(PT_Binop, OP_Divide)