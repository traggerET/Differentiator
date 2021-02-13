#pragma once
#include "../Data.hpp"
#include "../Classes/Class_declaring.hpp"
//tex functions
void writeEquation(FILE *fname, Node *cur_node, Node *parent);
void sub_op_proc(FILE *fname, Node* cur_node, Node* parent);
void add_op_proc(FILE *fname, Node* cur_node, Node *parent);
void mul_op_proc(FILE *fname, Node* cur_node);
void div_op_proc(FILE *fname, Node* cur_node);
void pow_op_proc(FILE *fname, Node *cur_node);
void un_op_proc(FILE *fname, Node *cur_node);
void writeFuncAndHisDiff(Tree* diff_tree, const char* tex_filename);
void writeTexBegin(FILE* file);
void writeTexEnd(FILE* file);
void systemCall(const char* tex_filename);

//Node* parse_eq(char** cur_pos);
OP_CODE choose_Uoperation(char *word);
OP_CODE choose_Boperation(char *sign);

void printNodeValue(FILE* file, Node* node);
void graphRepoter(Node* node, const char* reason);
void drawGraph(Node* node, const char* filename);
Node* loadNudes(FILE* file, Node* node);
void systemCall(const char* filename, const char* graph_filename);

//overload func. and operators
Node *operator+(Node &lh, Node &rh); 
Node *_const(double new_val);
Node *operator*(Node &lh, Node &rh);
Node *operator-(Node &lh, Node &rh);
Node *operator/(Node &lh, Node &rh);
Node *operator^(Node &lh, Node &rh);
Node *cos(Node &rh);
Node *sin(Node &rh);
Node *tan(Node &rh);
Node *exp(Node &rh);
Node *log(Node &rh);
bool find_var(Node *node, unsigned var);

char* read_file(const char* fname);

double do_operation(Node *lnode, Node *rnode, int op_code);

bool is_add(Node *node);
bool is_sub(Node *node);
bool is_mul(Node *node);
bool is_div(Node *node);
bool is_pow(Node *node);
bool is_null(Node *node);
bool is_one(Node *node);
bool is_constant(Node *node);
bool is_var(Node *node);
bool is_op(Node *node);
