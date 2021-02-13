#pragma once

enum TYPE {
    TYPE_OP = 1001,
    TYPE_CONST,
    TYPE_VAR,
};
enum OP_CODE {
    OP_ADD = 0,
    OP_SUB,
    OP_MUL,
    OP_DIV,
    OP_POW,
    OP_SIN,
    OP_COS,
    OP_TAN,
    OP_LOG,
    OP_EXP,
    OP_OTHER,
};

const int EPS = 1e-6;
