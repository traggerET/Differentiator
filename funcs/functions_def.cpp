#include <string>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <cstring>
#include <cassert>
#include <cmath>
#include <cstdlib>
#include "functions_decl.hpp"

void writeTexBegin(FILE* file)
{
    fprintf(file, "\\documentclass{article}\n");

    fprintf(file, "\\usepackage[russian]{babel}\n"
                  "\\usepackage[utf8]{inputenc}\n"
                  "\\usepackage{amsmath}\n");

    fprintf(file, "\n\\begin{document}\n");
}

void writeTexEnd(FILE* file)
{
    fprintf(file, "\\end{document}");
}

void writeFuncAndHisDiff(Tree* diff_tree, const char* tex_filename)
{
    FILE* file = fopen(tex_filename, "w");

    writeTexBegin(file);

    fprintf(file, "\n$$\n");
    writeEquation(file, diff_tree->getRoot(), nullptr);
    fprintf(file, "\n$$\n");

    writeTexEnd(file);

    fclose(file);

    systemCall(tex_filename);
}

void writeEquation(FILE *fname, Node *cur_node, Node *parent) {
    
    if (is_pow(cur_node))
        pow_op_proc(fname, cur_node);

    else if (is_div(cur_node))
        div_op_proc(fname, cur_node);

    else if (is_mul(cur_node))
        mul_op_proc(fname, cur_node);
    
    else if (is_add(cur_node))
        add_op_proc(fname, cur_node, parent);

    else if (is_sub(cur_node))
        sub_op_proc(fname, cur_node, parent);

    else if (is_var(cur_node)) {
        //printf("%c\n", (unsigned int)cur_node->getVal() + 'a');
        fprintf(fname, "%c", (unsigned int)cur_node->getVal() + 'a');
    }

    else if (is_constant(cur_node)) 
        fprintf(fname, "%lg", cur_node->getVal());

    else //unary operations are left
        un_op_proc(fname, cur_node);
}

void un_op_proc(FILE *fname, Node *cur_node) {
    fprintf(fname, "\\%s(", ((OpNode*)cur_node)->getUnOpName());
    //going down to the next son (and the only one)
    writeEquation(fname, cur_node->getRch(), cur_node);
    fprintf(fname, ")");
}

void pow_op_proc(FILE *fname, Node *cur_node) {

    if (is_op(cur_node->getLch())) {
        fprintf(fname, "(");
        writeEquation(fname, cur_node->getLch(), cur_node);
        fprintf(fname, ")^{");
        writeEquation(fname, cur_node->getRch(), cur_node);
        fprintf(fname, "}");
    }
    else {
        writeEquation(fname, cur_node->getLch(), cur_node);
        fprintf(fname, "^{");
        writeEquation(fname, cur_node->getRch(), cur_node);
        fprintf(fname, "}");
    }
}

void div_op_proc(FILE *fname, Node* cur_node) {
    fprintf(fname, "\\frac{");
    writeEquation(fname, cur_node->getLch(), cur_node);
    fprintf(fname, "}{");
    writeEquation(fname, cur_node->getRch(), cur_node);
    fprintf(fname, "}");
}

void mul_op_proc(FILE *fname, Node* cur_node) {
    writeEquation(fname, cur_node->getLch(), cur_node);
    writeEquation(fname, cur_node->getRch(), cur_node);
}

void add_op_proc(FILE *fname, Node* cur_node, Node *parent) {
    if (is_mul(parent)) {
        fprintf(fname, "(");
        writeEquation(fname, cur_node->getLch(), cur_node);
        fprintf(fname, "+");
        writeEquation(fname, cur_node->getRch(), cur_node);
        fprintf(fname, ")");
    }
    else {
        writeEquation(fname, cur_node->getLch(), cur_node);
        fprintf(fname, "+");
        writeEquation(fname, cur_node->getRch(), cur_node);
    }
}

void sub_op_proc(FILE *fname, Node* cur_node, Node* parent) {
    if (is_mul(parent)) {
        fprintf(fname, "(");
        writeEquation(fname, cur_node->getLch(), cur_node);
        fprintf(fname, "-");
        writeEquation(fname, cur_node->getRch(), cur_node);
        fprintf(fname, ")");
    }
    else {
        writeEquation(fname, cur_node->getLch(), cur_node);
        fprintf(fname, "-");
        writeEquation(fname, cur_node->getRch(), cur_node);
    }
}


OP_CODE choose_Boperation(char *sign) {
    switch (*sign) {
        case '+':
            return OP_ADD;
        case '-':
            return OP_SUB;
        case '*':
            return OP_MUL;
        case '/':
            return OP_DIV;
        case '^':
            return OP_POW;
        default:
            printf("not found appropriate bin op");
            return OP_OTHER;
    }
}

OP_CODE choose_Uoperation(char *word) {
    if (strcmp(word, "sin") == 0 ) 
        return OP_SIN;
    else if (strcmp(word, "tan") == 0) 
        return OP_TAN;
    else if (strcmp(word, "cos") == 0)
        return OP_COS;
    else if (strcmp(word, "exp") == 0)
        return OP_EXP;
    else if (strcmp(word, "log") == 0)
        return OP_LOG;
    else {
        printf("cant find appropriate unary operation");
        return OP_OTHER;
    }
}   

//overload func. and operators
Node *_const(double new_val) {
    return new ConstNode(TYPE_CONST, new_val, nullptr, nullptr);
}
Node *operator+(Node &lh, Node &rh) {
    return new OpNode(TYPE_OP, OP_ADD, &lh, &rh);
}
Node *operator*(Node &lh, Node &rh) {
    return new OpNode(TYPE_OP, OP_MUL, &lh, &rh);
}
Node *operator-(Node &lh, Node &rh) {
    return new OpNode(TYPE_OP, OP_SUB, &lh, &rh);
}
Node *operator/(Node &lh, Node &rh) {
    return new OpNode(TYPE_OP, OP_DIV, &lh, &rh);
}
Node *operator^(Node &lh, Node &rh) {
    return new OpNode(TYPE_OP, OP_POW, &lh, &rh);
}
Node *cos(Node &rh) {
    return new OpNode(TYPE_OP, OP_COS, nullptr, &rh);
}
Node *sin(Node &rh) {
    return new OpNode(TYPE_OP, OP_SIN, nullptr, &rh);
}
Node *tan(Node &rh) {
    return new OpNode(TYPE_OP, OP_TAN, nullptr, &rh);
}
Node *exp(Node &rh) {
    return new OpNode(TYPE_OP, OP_EXP, nullptr, &rh);
}
Node *log(Node &rh) {
    return new OpNode(TYPE_OP, OP_LOG, nullptr, &rh);
}
bool find_var(Node *node, unsigned int var) {
    if (node == nullptr) 
        return 0;
    if (node->getType() == TYPE_CONST)
        return 0;
    if (node->getType() == TYPE_OP) 
        return std::max(find_var(node->getLch(), var), find_var(node->getRch(), var));
    else 
        return node->getVal() == var;
}

char* read_file(const char* fname) {
    FILE* fin = fopen(fname, "r");
    assert(fin);
    fseek(fin, 0L, SEEK_END);
    //+1 for \0
    size_t size = ftell(fin) + 1; 
    fseek(fin, 0L, SEEK_SET);
    char* buf = (char*)calloc(size, sizeof(char));
    fread(buf, 1, size, fin);
    return buf;
}

double do_operation(Node *lnode, Node *rnode, int op_code) {
    switch (op_code) {
        case OP_ADD:
            return lnode->getVal() + rnode->getVal();
        case OP_SUB:
            return lnode->getVal() - rnode->getVal();
        case OP_MUL:
            return lnode->getVal() * rnode->getVal();
        case OP_DIV:
            if (abs(rnode->getVal()) > EPS)
                return lnode->getVal() / rnode->getVal();
            else {
                printf("Emergency exit: dividing by 0\n");
                return 0;
            }
        case OP_POW:
            return pow(lnode->getVal(), rnode->getVal());
        case OP_SIN:
            return sin(rnode->getVal());
        case OP_COS:
            return cos(rnode->getVal());
        case OP_TAN:
            return tan(rnode->getVal());
        case OP_LOG:
            if (rnode->getVal() > 0)
                return log(rnode->getVal());
            else {
                printf("Emergency exit: log(val), val <= 0\n");
                return 0;
            }
        case OP_EXP:
            return exp(rnode->getVal());
        default:
            printf("cant simplify const expression, appropriate operation not found\n");
            return 0;
    }
}

bool is_add(Node *node) {
    return node != nullptr && node->getType() == TYPE_OP && node->getVal() == OP_ADD;
}
bool is_sub(Node *node) {
    return node != nullptr && node->getType() == TYPE_OP && node->getVal() == OP_SUB;
}
bool is_mul(Node *node) {
    return node != nullptr && node->getType() == TYPE_OP && node->getVal() == OP_MUL;
}
bool is_div(Node *node) {
    return node != nullptr && node->getType() == TYPE_OP && node->getVal() == OP_DIV;
}
bool is_pow(Node *node) {
    return node != nullptr && node->getType() == TYPE_OP && node->getVal() == OP_POW;
}
bool is_null(Node *node) {
    return node != nullptr && node->getType() == TYPE_CONST && node->getVal() == 0;
}
bool is_one(Node *node) {
    return node != nullptr && node->getType() == TYPE_CONST && node->getVal() == 1;
}
bool is_constant(Node *node) {
    return node != nullptr && node->getType() == TYPE_CONST;
}
bool is_var(Node *node) {
    return node != nullptr && node->getType() == TYPE_VAR;
}
bool is_op(Node *node) {
    return node != nullptr && node->getType() == TYPE_OP;
}


void systemCall(const char* tex_filename)
{
    const char* TEX_COMMAND = "pdflatex ";
    char command[96] = {};

    strcpy(command, TEX_COMMAND);
    strcat(command, tex_filename);

    system(command);
    
}

void systemCall(const char* filename, const char* graph_filename)
{
    const char* DOT_COMMAND = "dot -Tpng ";
    char command[96] = {};
    size_t i = 0;
    while (DOT_COMMAND[i] != '\0')
    {
        command[i] = DOT_COMMAND[i];
        ++i;
    }
    size_t j = 0;
    while (filename[j] != '\0')
    {
        command[i] = filename[j++];
        ++i;
    }
    command[i++] = ' ';
    command[i++] = '-';
    command[i++] = 'o';
    command[i++] = ' ';
    j = 0;
    while (graph_filename[j] != '\0')
    {
        command[i] = graph_filename[j++];
        ++i;
    }
    system(command);
}

void graphRepoter(Node* node, const char* reason)
{
    static size_t number_of_calls = 0;
    printf("====\nGRAPH REPORTER CALL #%d\n======\n", number_of_calls);

    char filename[128];
    
    snprintf(filename, 128, "Nodes/%s%d.png", reason, number_of_calls);
    drawGraph(node, filename);

    ++number_of_calls;
}

void drawGraph(Node* node, const char* filename)
{
    if (node == nullptr) 
    {
        return;
    }
    FILE* file = fopen("graphPlot.txt", "w");

    fprintf(file, "digraph G{\n");
    fprintf(file, "node [shape=\"record\", style=\"solid\", color=\"blue\"];\n");

    loadNudes(file, node);

    fprintf(file, "}");

    fclose(file);

    systemCall("graphPlot.txt", filename);
}

Node* loadNudes(FILE* file, Node* node)
{
    if (node == nullptr)
    {
        return nullptr;
    }
    if (node->getLch() != nullptr)
    {
        fprintf(file, "\"%p\":sw->\"%p\"\n", node, node->getLch());  // link parent and left child
        loadNudes(file, node->getLch());
    }
    if (node->getRch() != nullptr)
    {
        fprintf(file,"\"%p\":se->\"%p\"\n", node, node->getRch()); //link parent and right child
        loadNudes(file, node->getRch());
    }
    printNodeValue(file, node);
    return node;
}

void printNodeValue(FILE* file, Node* node)
{   
    const char* OPERS[] = 
    {"+", "-", "*", "/", "^", "sin", "cos", "tan", "log", "exp",};

    switch (node->getType())
    {
    case TYPE_CONST:
        fprintf(file, "\"%p\" [style=\"filled\", fillcolor=\"seagreen1\", label=\"{%lg}\"]\n ", node, node->getVal()); 
        break;

    case TYPE_VAR:
        fprintf(file, "\"%p\" [style=\"filled\", fillcolor=\"lightpink\", label=\"{%c}\"]\n ", node, (int)node->getVal() + 'a'); 
        break;
    
    case TYPE_OP:
        fprintf(file, "\"%p\" [style=\"filled\", fillcolor=\"skyblue1\", label=\"{%s}\"]\n ", node, OPERS[(int)node->getVal()]);
        break;
    default:
        printf("undefined behavior, appropriate type in printnodevalue not found\n");
        //fprintf(file, "\"%p\" [style=\"filled\", fillcolor=\"seashell1\", label=\"{void}\"]\n ", node);
    }
}

