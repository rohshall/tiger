#ifndef ZIR_TREE
#define ZIR_TREE

enum class T_binOp{
  T_plus, T_minus, T_mul, T_div,T_and, T_or, T_lshift, T_rshift, T_arshift, T_xor, eNumCount
};

enum class T_relOp{
  T_eq, T_ne, T_lt, T_gt, T_le, T_ge,T_ult, T_ule, T_ugt, T_uge, eNumCount
};

#endif
