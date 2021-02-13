#include <string>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <cstring>
#include <cassert>
#include <cmath>
#include <cstdlib>

enum TYPE {
    TYPE_OP = 1001,
    TYPE_CONST,
    TYPE_VAR,
};
const char* OPERS[] = 
{
    "+",
    "-",
    "*",
    "/",
    "^",

    "sin",
    "cos",
    "tan",

    "log",
    "exp",
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

using namespace std;

struct var_list {
    unsigned char ind;
    char var_name;
};

char var_list[26];
const char* DOT_COMMAND = "dot -Tpng ";
const char* TEX_COMMAND = "pdflatex ";

const int EPS = 1e-6;

class Node {
public:
virtual ~Node() {}

virtual Node *clone() = 0;
virtual void copy(Node *c_node) {
    if (c_node->left != nullptr) 
        left = c_node->left->clone();
    else 
        left = nullptr;
    if (c_node->right != nullptr) 
        right = c_node->right->clone();
    else
        right = nullptr;
    type = c_node->type;
    val = c_node->val;
    //parent = c_node->parent;
}

Node *getRch() {
    return right;
}
Node **getRchptr() {
    return &right;
}
Node *getLch() {
    return left;
}
Node **getLchptr() {
    return &left;
}
void setLeft(Node *ptr) {
    left = ptr;
}
void setRight(Node *ptr) {
    right = ptr;
}

unsigned int getType() {
    return type;
}

double getVal() {
    return val;
}
//differentiation function
virtual Node *d(unsigned int var) = 0;

protected:
//Node *parent;
Node *left;
Node *right;
unsigned type;
double val;
private:

};
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


class OpNode : public Node {
private: 
    
public:
    OpNode(unsigned int type_op = 0, double op_num = 0, Node *lptr = nullptr, Node *rptr = nullptr) {
        type = type_op;
        val = op_num;
        left = lptr;
        right = rptr;
    }

    virtual ~OpNode() {
        delete right;
        delete left;
    }

    const char *getUnOpName() {
        switch ((unsigned)val) {
            case OP_EXP:
                return "exp";
            case OP_TAN:
                return "tan";
            case OP_SIN:
                return "sin";
            case OP_COS:
                return "cos";
            case OP_LOG:
                return "log";
            default: 
                printf("unary op not found");
                return nullptr;
        }
    }

    virtual Node *clone() {
        OpNode *node = new OpNode();
        node->copy(this);
        return node;
    }
    virtual void copy(Node *c_node) {
        Node::copy(c_node);
    }

    #define dL (*(left->d(dif_val)))
    #define dR (*(right->d(dif_val)))
    #define cL (*(left->clone()))
    #define cR (*(right->clone()))
    //differentiation function
    virtual Node *d(unsigned int dif_val) {
        switch ((unsigned)val) {
            case OP_ADD:
                return dL + dR;
            case OP_SUB:
                return dL - dR;
            case OP_MUL:
                return *(dL * cR) + *(cL * dR);
            case OP_DIV: 
                return (*(*(dL * cR)  - *(cL * dR))) / (*(cR ^ (*_const(2))));
            case OP_SIN:
                return (*cos(cR)) * dR;
            case OP_COS:
                return (*((*_const(-1)) * (*sin(cR)))) * dR; 
            case OP_EXP:
                return (*exp(cR)) * dR;
            case OP_TAN:
                return dR / (*((*cos(cR)) ^ (*_const(2))));
            //it is LN actually
            case OP_LOG:
                return dR / cR ;
            case OP_POW:
                //if has no var in its power, calculate as power function
                if (!find_var(right, dif_val))
                    return *(cR * (*(cL ^ (*(cR - *_const(1)))))) * dL;
                //if has in power but has not in basement, calculate as exponential
                else if (!find_var(left, dif_val))
                    return *(*log(cL) * dR) * (*(cL ^ cR));
                else 
                    //calculates derivative of f(x)^g(x) - power-exponential f.
                    return *(*(cR * (*(cL ^ (*(cR - *_const(1)))))) * dL) + *(*(*log(cL) * dR) * (*(cL ^ cR)));
            default: 
                return nullptr;
        }  
    }
    #undef dL 
    #undef dR 
    #undef cL 
    #undef cR 

};

class ConstNode : public Node {
private:
    double val;
public:    
    virtual Node *d(unsigned int dif_val) {
        return new ConstNode(TYPE_CONST, 0, nullptr, nullptr);
    }
    
    ConstNode(unsigned int type_const = 0, double new_val = 0, Node *lptr = nullptr, Node *rptr = nullptr){
        type = type_const;
        val = new_val;
        left = lptr;
        right = rptr;
    }
    virtual ~ConstNode() {
        delete right;
        delete left;
    }  

    virtual Node *clone() {
        ConstNode *node = new ConstNode();
        node->copy(this);
        return node;
    }
    virtual void copy(Node *c_node) {
        Node::copy(c_node);
    }
};

class VarNode : public Node {
private:
public: 
    virtual Node *d(unsigned int dif_val) {
        if ((unsigned)val == dif_val)
            return new ConstNode(TYPE_CONST, 1, nullptr, nullptr);
        else 
            return new ConstNode(TYPE_CONST, 0, nullptr, nullptr);
    }
    VarNode(unsigned int type_var = 0, double new_val = 0, Node *lptr = nullptr, Node *rptr = nullptr) {
        val = new_val;
        type = type_var;
        left = lptr;
        right = rptr;
    }
    virtual ~VarNode() {
        delete right;
        delete left;
    }

    virtual Node *clone() {
        VarNode *node = new VarNode();
        node->copy(this);
        return node;
    }
    virtual void copy(Node *c_node) {
        Node::copy(c_node);
    }
};

//Node* parse_eq(char** cur_pos);
OP_CODE choose_Uoperation(char *word);
OP_CODE choose_Boperation(char *sign);

//OVERLOAD OPERATORS
Node *_const(double new_val) {
    return new ConstNode(TYPE_CONST, new_val, nullptr, nullptr);
}
Node *operator+(Node &lh, Node &rh) {
    return new OpNode(TYPE_OP, (double)OP_ADD, &lh, &rh);
}
Node *operator*(Node &lh, Node &rh) {
    return new OpNode(TYPE_OP, (double)OP_MUL, &lh, &rh);
}
Node *operator-(Node &lh, Node &rh) {
    return new OpNode(TYPE_OP, (double)OP_SUB, &lh, &rh);
}
Node *operator/(Node &lh, Node &rh) {
    return new OpNode(TYPE_OP, (double)OP_DIV, &lh, &rh);
}
Node *operator^(Node &lh, Node &rh) {
    return new OpNode(TYPE_OP, (double)OP_POW, &lh, &rh);
}
Node *cos(Node &rh) {
    return new OpNode(TYPE_OP, (double)OP_COS, nullptr, &rh);
}
Node *sin(Node &rh) {
    return new OpNode(TYPE_OP, (double)OP_SIN, nullptr, &rh);
}
Node *tan(Node &rh) {
    return new OpNode(TYPE_OP, (double)OP_TAN, nullptr, &rh);
}
Node *exp(Node &rh) {
    return new OpNode(TYPE_OP, (double)OP_EXP, nullptr, &rh);
}
Node *log(Node &rh) {
    return new OpNode(TYPE_OP, (double)OP_LOG, nullptr, &rh);
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

//skip symbols are forbidden
//() - empty constructions are forbidden

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

bool find_var(Node *node, unsigned int var) {
    if (node == nullptr) 
        return 0;
    if (node->getType() == TYPE_CONST)
        return 0;
    if (node->getType() == TYPE_OP) 
        return max(find_var(node->getLch(), var), find_var(node->getRch(), var));
    else 
        return (unsigned)node->getVal() == var;
}

bool is_add(Node *node) {
    return node != nullptr && node->getType() == TYPE_OP && (unsigned)node->getVal() == OP_ADD;
}
bool is_sub(Node *node) {
    return node != nullptr && node->getType() == TYPE_OP && (unsigned)node->getVal() == OP_SUB;
}
bool is_mul(Node *node) {
    return node != nullptr && node->getType() == TYPE_OP && (unsigned)node->getVal() == OP_MUL;
}
bool is_div(Node *node) {
    return node != nullptr && node->getType() == TYPE_OP && (unsigned)node->getVal() == OP_DIV;
}
bool is_pow(Node *node) {
    return node != nullptr && node->getType() == TYPE_OP && (unsigned)node->getVal() == OP_POW;
}
bool is_null(Node *node) {
    return node != nullptr && node->getType() == TYPE_CONST && node->getVal() < EPS;
}
bool is_one(Node *node) {
    return node != nullptr && node->getType() == TYPE_CONST && (node->getVal() - 1 < EPS);
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

//The function gets not values but pointers because it's possible a case
//when lnode will be null ptr(if operation is unary). 
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

class Tree {
    private:
    Node *root;
    Node* parse_eq(char** cur_pos) {
    //printf("(%c)   ", **cur_pos);
    if (**cur_pos == '(') {
        (*cur_pos)++;

        //if we have '((' then we have expression of the kind: ((...) + (...))
        //if there is a subtree
        if (**cur_pos == '(') {
            //get left participant of arithmetical expression
            Node* l_node = parse_eq(cur_pos);
            //printf("l_node\n");
            //get arithmetical operation (^+-/*) and move on 
            unsigned int op_code = (unsigned int)choose_Boperation((*cur_pos)++);
            //printf("op_code = %u\n", op_code);
            //get right part-t of ar-cal exp.
            Node* r_node = parse_eq(cur_pos);
            //printf("r_node\n");
            //skip ')'
            (*cur_pos)++;
            //build node and return
            return new OpNode(TYPE_OP, (double)op_code, l_node, r_node);
        }

        //if it is positive or neg. number  
        else if (isdigit(**cur_pos) || (isdigit(*(*cur_pos + 1)) && **cur_pos == '-')) {
            double num = atof(*cur_pos);
            //skip with ')'
            while (*((*cur_pos)++) != ')');
            //printf("double num = %f\n", num);
            return new ConstNode(TYPE_CONST, num, nullptr, nullptr);
        }

        //if it is a variable
        else if (isalpha(**cur_pos) && !isalpha(*(*cur_pos + 1))) {
            //searching for var index in var_list
            size_t pos_in_arr = **cur_pos - 'a';
            var_list[pos_in_arr]++;
            //printf("var = %zu\n", pos_in_arr);
            //skip var and ')'
            *cur_pos += 2;
            return new VarNode(TYPE_VAR, (double)pos_in_arr, nullptr, nullptr);
        }

        //only un operations and logarithm are left (sin, cos, ...) 
        else {
            char *tmp = strchr(*cur_pos, '(');
            char last = *tmp;
            *tmp = '\0';
            //printf("un operation is %s\n", *cur_pos);
            //returns code of operation
            unsigned int op_code = (unsigned int)choose_Uoperation(*cur_pos);
            *tmp = last;
            //tmp is currently at '('
            *cur_pos = tmp;
            Node *tmp_node = new OpNode(TYPE_OP, (double)op_code, nullptr, parse_eq(cur_pos));
            //skip ')' that is responsible for closing (sin())
            (*cur_pos)++;
            return tmp_node;
        }
    }
    else {
        printf("not found appropriate lexem");
        return nullptr;
    }
}


    //returns true if there has been made simplifying in the tree; false otherwise
    bool simplify_const(Node **node) {
        bool ltick = false;
        bool rtick = false;
    //if l branch exists and is op. -> go down there recursively
    if ((*node)->getLch() != nullptr && (*node)->getLch()->getType() == TYPE_OP)
        ltick = simplify_const((*node)->getLchptr());
    //the same with right branch
    if ((*node)->getRch() != nullptr && (*node)->getRch()->getType() == TYPE_OP)
        rtick = simplify_const((*node)->getRchptr());
    //if left is nullptr and right is const, we have (un_op(const))
    //if both child are const, we have (const (bin_op) const) 
    //in both cases we can simplify it
    if  (((*node)->getLch() == nullptr || (*node)->getLch()->getType() == TYPE_CONST) && (*node)->getRch() != nullptr && (*node)->getRch()->getType() == TYPE_CONST) {
        double new_value = do_operation((*node)->getLch(), (*node)->getRch(), (*node)->getVal());
        delete *node;
        *node = new ConstNode(TYPE_CONST, new_value, nullptr, nullptr);
        return true;
    }
    else 
        return ltick || rtick;
}

    bool mul_ntrl_process(Node **node) {
        if (is_null((*node)->getLch()) || is_null((*node)->getRch())) {
            delete *node;
            *node = new ConstNode(TYPE_CONST, 0, nullptr, nullptr);
            return true;
        }
        else if (is_one((*node)->getRch())) {
            hangtoLeft(node);
            return true;
        }
        else if (is_one((*node)->getLch())) {
            hangtoRight(node);
            return true;
        }
        else 
            return false;
    }
    bool add_ntrl_process(Node **node) {
        if (is_null((*node)->getLch())) {
            hangtoRight(node);
            return true;
        }
        else if (is_null((*node)->getRch())) {
            hangtoLeft(node);
            return true;
        }
        else 
            return false;
    }
    bool sub_ntrl_process(Node **node) {
        if (is_null((*node)->getRch())) {
            hangtoLeft(node);
            return true;
        }
        else 
            return false;
    }
    bool div_ntrl_process(Node **node) {
        if (is_one((*node)->getRch())) {
            hangtoLeft(node);
            return true;
        }
        else 
            return false;
    }
    bool pow_ntrl_process(Node **node) {
        if (is_one((*node)->getLch())) {
            delete *node;
            *node = new ConstNode(TYPE_CONST, 1, nullptr, nullptr);
            return true;
        }
        else if (is_one((*node)->getRch())) {
            hangtoLeft(node);
            return true;
        }
        else 
            return false;
    }

    void hangtoLeft(Node **node) {
        Node* tmpnode = (*node)->getLch();
        (*node)->setLeft(nullptr);
        delete *node;
        *node = tmpnode;
    }
    void hangtoRight(Node **node) {
        Node* tmpnode = (*node)->getRch();
        (*node)->setRight(nullptr);
        delete *node;
        *node = tmpnode; 
    }
    bool process_neutral(Node **node) {
    bool ltick = false;
    bool rtick = false;
    bool curtick = false;
    if (is_mul(*node)) 
        curtick = mul_ntrl_process(node);

    else if (is_add(*node)) 
        curtick = add_ntrl_process(node);

    else if (is_sub(*node)) 
        curtick = sub_ntrl_process(node);

    else if (is_div(*node)) 
        curtick = div_ntrl_process(node);

    else if (is_pow(*node)) 
        curtick = pow_ntrl_process(node);
    
    //if child is operation, go down recursively in that child
    if ((*node)->getLch() != nullptr && (*node)->getLch()->getType() == TYPE_OP)
        ltick = process_neutral((*node)->getLchptr());
    if ((*node)->getRch() != nullptr && (*node)->getRch()->getType() == TYPE_OP)
        rtick = process_neutral((*node)->getRchptr());  
    return curtick || ltick || rtick;
}
    
    protected:

    public:
    //constructs empty tree if fbegin_ == nullptr, if not - reads from buff and create a real tree 
    Tree(char **fbegin_ = nullptr) {
        if (fbegin_ != nullptr)
            root = parse_eq(fbegin_);
    }
    Node *diff_by_var(unsigned int var) {
        return root->d(var);
    }
    //simplifies tree until it's perfect
    void TakeCare_of_Tree() {
        while (simplify_const(&root) || process_neutral(&root));
    }

    Node *getRoot() {
        return root;
    }

    void setRoot(Node *ptr) {
        root = ptr;
    }
    Node **getRootptr() {
        return &root;
    }
    ~Tree() {
        delete root;
    }
};

void printNodeValue(FILE* file, Node* node);
void graphRepoter(Node* node, const char* reason);
void drawGraph(Node* node, const char* filename);
Node* loadNudes(FILE* file, Node* node);
void systemCall(const char* filename, const char* graph_filename);
void systemCall(const char* tex_filename);

void systemCall(const char* tex_filename)
{
    char command[96] = {};

    strcpy(command, TEX_COMMAND);
    strcat(command, tex_filename);

    system(command);
    
}

void systemCall(const char* filename, const char* graph_filename)
{
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
void writeEquation(FILE *fname, Node *cur_node, Node *parent);
void sub_op_proc(FILE *fname, Node* cur_node, Node* parent);
void add_op_proc(FILE *fname, Node* cur_node, Node *parent);
void mul_op_proc(FILE *fname, Node* cur_node);
void div_op_proc(FILE *fname, Node* cur_node);
void pow_op_proc(FILE *fname, Node *cur_node);
void un_op_proc(FILE *fname, Node *cur_node);


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

int main() {
    char *buf = read_file("eq.txt");
    Tree *tree = new Tree(&buf);
    drawGraph(tree->getRoot(), "Graph.png");
    Tree *diff_tree = new Tree();
    diff_tree->setRoot(tree->diff_by_var('x' - 'a'));
    diff_tree->TakeCare_of_Tree();
    drawGraph(diff_tree->getRoot(), "DiffGraph.png");
    writeFuncAndHisDiff(diff_tree, "tex.txt");
    delete tree;
    delete diff_tree;
    printf("happy\n");
    return 0;
}