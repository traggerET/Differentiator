
#include <string>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <cstring>
#include <cassert>
#include <cmath>

enum TYPE {
    TYPE_OP = 1001,
    TYPE_CONST,
    TYPE_VAR,
};

enum OP_CODE {
    OP_ADD = 250,
    OP_SUB,
    OP_DIV,
    OP_MUL,
    OP_POW,
    OP_COS,
    OP_SIN,
    OP_TG,
    OP_LOG,
    OP_EXP,
};

using namespace std;

struct var_list {
    unsigned char ind;
    char var_name;
};

const char var_list[] = {'a', 'b', 'c', 'd', 'e',
                        'f', 'g', 'h', 'i', 'j',
                        'k', 'l', 'm', 'n', 'o',
                        'p', 'r', 's', 't', 'u', 
                        'v','w', 'x', 'y', 'z'};
                        class Node {
class Node {
public:
Node *operator+(Node &rh);
/*Node(Node *new_p, Node *new_Lch, Node *new_Rch, unsigned int new_t): 
    parent(new_p), left(new_Lch), right(new_Rch) {
}*/
//virtual Node *new_node() = 0;

virtual Node *clone() = 0;
virtual void copy(Node *c_node);

Node *getParent();
//get Right child ptr
Node *getRch();
Node *getLch();
virtual const unsigned int getType() = 0;

virtual const double getVal() = 0;
//differentiation function
virtual Node *d() = 0;

protected:
Node *parent;
Node *left;
Node *right;

private:

};

class Node {
public:
Node *operator+(Node &rh) {
    return new OpNode::OpNode(TYPE_OP, OP_ADD, this, &rh);
}
/*Node(Node *new_p, Node *new_Lch, Node *new_Rch, unsigned int new_t): 
    parent(new_p), left(new_Lch), right(new_Rch) {
}*/
//virtual Node *new_node() = 0;

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
    parent = c_node->parent;
}

Node *getParent() {
    return parent;
}
//get Right child ptr
Node *getRch() {
    return right;
}
Node *getLch() {
    return left;
}
virtual const unsigned int getType() = 0;

virtual const double getVal() = 0;
//differentiation function
virtual Node *d() = 0;

protected:
Node *parent;
Node *left;
Node *right;

private:

};

class OpNode : public Node {
private: 
    unsigned int val;
    unsigned int type;
    
public:
    OpNode(unsigned int type_op, unsigned int op_num, Node *lptr, Node *rptr){
        type = type_op;
        val = op_num;
        Node::left = lptr;
        Node::right = rptr;
    }
    OpNode(){
        type = 0;
        val = 0;
        Node::left = nullptr;
        Node::right = nullptr;
    }
    virtual const unsigned int getType() {
        return type;
    }
    virtual const double getVal() {
        return (double)val;
    }   


    /*OpNode(Node *new_p, Node *new_Lch, Node *new_Rch, unsigned int new_t, double new_v) :
    Node(new_p, new_Lch, new_Rch, new_t), val(new_v){
    }*/
    /*virtual Node *new_node(Node *new_p, Node *new_Lch, Node *new_Rch, unsigned int new_t, unsigned int new_v) {
        return new OpNode(new_p, new_Lch, new_Rch, new_t, new_v);
    }*/
    virtual Node *clone() {
        OpNode *node = new OpNode();
        node->copy(this);
        return node;
    }
    virtual void copy(Node *c_node) {
        Node::copy(c_node);
        type = ((OpNode*)c_node)->type;
        val = ((OpNode*)c_node)->val;
    }

    /*
    virtual new_node() {
        return 
    }*/
    

    #define dL (left->d())
    #define dR (right->d())
    #define cL (left->clone())
    #define cR (right->clone())
    //differentiation function
    virtual Node *d() {
        switch (val) {
            
            case OP_ADD:
                return *dL + *dR;/*
            case OP_SUB:
                return dL - dR;
            case OP_MUL:
                return dL * cR + cL * dR;
            case OP_DIV: 
                return (dL * cR  - cL * dR) / (cR * cR);
            case OP_SIN:
                return cos(cR) * dR;
            case OP_COS:
                return _const(-1) * sin(cR) * dR; 
            case OP_EXP:
                return exp(cR) * dR;
            case OP_TG:
                return _const(1) / (cos(cR) * cos(cR)) * dR;
            //it is LN actually
            case OP_LOG:
                return _const(1) / cR * dR;
            case OP_POW:
                //if has no var in its power, calculate as power function
                //if (!find_var(node->right, var))
                //    return cR * (cL ^ (cR - _const(1))) * dL;
                //if has in power but has not in basement, calculate as exponential
                //else if (!find_var(node->left, var))
                //    return log(cL) * dR * (cL ^ cR);
                //else 
                    //calculates derivative of f(x)^g(x) - power-exponential f.
                    return cR * (cL ^ (cR - _const(1))) * dL + log(cL) * dR * (cL ^ cR);*/
            default: 
                return nullptr;
        }  
    }

//++++ overloads:
/*
Node *operator*(Node *rh) {
    return new OpNode(TYPE_OP, OP_MUL, this, rh);
}
Node *operator-(Node *rh) {
    return new OpNode(TYPE_OP, OP_SUB, this, rh);
}
Node *operator/(Node *rh) {
    return new OpNode(TYPE_OP, OP_DIV, this, rh);
}
Node *operator^(Node *rh) {
    return new OpNode(TYPE_OP, OP_POW, this, rh);
}
Node *cos() {
    return new OpNode(TYPE_OP, OP_COS, nullptr, this);
}
Node *sin() {
    return new OpNode(TYPE_OP, OP_SIN, nullptr, this);
}
Node *tg() {
    return new OpNode(TYPE_OP, OP_TG, nullptr, this);
}
Node *exp() {
    return new OpNode(TYPE_OP, OP_EXP, nullptr, this);
}
Node *log() {
    return new OpNode(TYPE_OP, OP_LOG, nullptr, this);
}*/

};

class ConstNode : public Node {
private:
    double val;
    unsigned int type;
public:    
    virtual Node *d() {return nullptr;}
    
    /*ConstNode() {
        val = copy.val;
    }*/
    ConstNode(unsigned int type_op, unsigned int op_num, Node *lptr, Node *rptr){
        type = type_op;
        val = op_num;
        Node::left = lptr;
        Node::right = rptr;
    }
    ConstNode(){
        type = 0;
        val = 0;
        Node::left = nullptr;
        Node::right = nullptr;
    }
    virtual const unsigned int getType() {
        return type;
    }
    virtual const double getVal() {
        return (double)val;
    }   

    Node *_const(double new_val) {
        return new ConstNode(TYPE_CONST, new_val, nullptr, nullptr);
    }
    /*ConstNode(Node *new_p, Node *new_Lch, Node *new_Rch, unsigned int new_t, unsigned int new_v) :
    Node(new_p, new_Lch, new_Rch, new_t),  val(new_v){
    }
    virtual Node *new_node(Node *new_p, Node *new_Lch, Node *new_Rch, unsigned int new_t, unsigned int new_v) {
        return new ConstNode(new_p, new_Lch, new_Rch, new_t, new_v);
    }*/

    virtual Node *clone() {
        ConstNode *node = new ConstNode();
        node->copy(this);
        return node;
    }
    virtual void copy(Node *c_node) {
        Node::copy(c_node);
        type = ((ConstNode*)c_node)->type;
        val = ((ConstNode*)c_node)->val;
    }
    /*virtual const double getVal() {
        return val;
    }*/
    
};

class VarNode : public Node {
private:
    unsigned int val;
    unsigned int type;
public: 
    virtual Node *d() {return nullptr;}
    /*VarNode(const VarNode& copy) : Node(copy) {
        val = copy.val;
    }*/
    virtual const unsigned int getType() {
        return type;
    }
    virtual const double getVal() {
        return (double)val;
    }   
    /*VarNode(Node *new_p, Node *new_Lch, Node *new_Rch, unsigned int new_t, unsigned int new_v) :
    Node(new_p, new_Lch, new_Rch, new_t),  val(new_v){
    }
    virtual Node *new_node(Node *new_p, Node *new_Lch, Node *new_Rch, unsigned int new_t, unsigned int new_v) {
        return new Varnode(new_p, new_Lch, new_Rch, new_t, new_v);
    }*/
    virtual Node *clone() {
        VarNode *node = new VarNode();
        node->copy(this);
        return node;
    }
    virtual void copy(Node *c_node) {
        Node::copy(c_node);
        type = ((VarNode*)c_node)->type;
        val = ((VarNode*)c_node)->val;
    }
};




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

/*create_node(int val_type, int new_val, Node* lson, Node* rson) {
    type = val_type;
    val = new_val;
    left = lson;
    right = rson;
}

unsigned char find_ind(char c) {
    unsigned char i = 0;
    for (i = 0; (var_list[i] != c) && (i < 26); i++);
    return i;
}

//skip symbols are forbidden
//() - empty constructions are forbidden
Node* parse_eq(char** cur_pos, Node* parent) {
    if (**cur_pos == '(') {
        (*cur_pos)++;

        //if we have '((' then we have expression of the kind: ((...) + (...))
        //if there is a subtree
        if (**cur_pos == '(') {
            //get left participant of arithmetical expression
            Node* l_node = parse_eq(cur_pos, new_node);
            //get arithmetical operation (^+-/*) and move on 
            int op_code = choose_Boperation((*cur_pos)++);
            //get right part-t of ar-cal exp.
            Node* r_node = parse_eq(cur_pos, new_node);
            //build node and return
            return new Node(TYPE_OP, op_code, l_node, r_node);
        }

        //if it is positive or neg. number  
        else if (isdigit(**cur_pos) || (isdigit(*(*cur_pos + 1)) && **cur_pos == '-')) {
            int num = atoi(*cur_pos);
            //skip with ')'
            if (**cur_pos == '-')
                cur_pos++;
            while (isdigit(*(*cur_pos)++));
            return new Node(TYPE_CONST, num, nullptr, nullptr);
        }

        //if it is a variable
        else if isalpha(**cur_pos) && !isalpha(*(*cur_pos + 1)) {
            //searching for var index in var_list
            size_t ind_var = find_ind(**cur_pos);
            if (ind_var == 26)
                return nullptr;
            //skip var and ')'
            *cur_pos += 2;
            return create_node(TYPE_VAR, ind_var, nullptr, nullptr);
        }

        //only un operations and logarithm are left (sin, cos, ...) 
        else {
            char* tmp = *cur_pos;
            while isalpha(*tmp) 
                tmp++;
            char* last = *tmp;
            *tmp = '\0';
            //returns code of operation
            int op_code = choose_Uoperation(*cur_pos);
            *tmp = last;
            //tmp is currently at '('
            *cur_pos = tmp;
            return create_node(TYPE_OP, op_code, nullptr, parse_eq(cur_pos));
        }
    }
}

(???) choose_Uoperation(char *word) {
    if (strcmp(word, "sqrt") == 1)
        return SQRT_CODE;
    else if (strcmp(word, "sin") == 1)
        return SIN_CODE;
    else if (strcmp(word, "cos") == 1)
        return COS_CODE;
    else if (strcmp(word, "tg") == 1)
        return TG_CODE;
    else if (strcmp(word, "ctg") == 1)
        return CTG_CODE;
    else if (strcmp(word, "arcsin") == 1)
        return ARCSIN_CODE;
    else if (strcmp(word, "arccos") == 1)
        return ARCCOS_CODE;
    else if (strcmp(word, "arctg") == 1)
        return ARCTG_CODE;
    else if (strcmp(word, "arcctg") == 1)
        return ARCCTG_CODE;
        }

//if it is un operation, print its name, put '(', argument, and ')'
void un_op_proc(Node *cur_node, FILE *fname) {
    //??
    fprintf(fname, "\\%s(", cur_node->get_name());
    //going down to the next son (and the only one)
    out_tree_tex(cur_node->right, fname);
    //??
    fprintf(fname, ")");
}

//if it is power operation 
void pow_op_proc(Node *cur_node, FILE *fname) {

    if (TYPE_IS(OP, cur_node->left)) {
        fprintf(fname, "(");
        out_tree_tex(cur_node->left, fname);
        fprintf(fname, ")^{");
        out_tree_tex(cur_node->right, fname);
        fprintf(fname, "}");
    }
    else {
        out_tree_tex(cur_node->left, fname);
        fprintf(fname, "^{");
        out_tree_tex(cur_node->right, fname);
        fprintf(fname, "}");
    }
    
}

void log_op_proc(Node *cur_node, FILE *fname) {
    //??
    fprintf(fname, "log_{");
    out_tree_tex(cur_node->left, fname);
    //??
    fprintf(fname, "}");
    //???
    if IS_OP(cur_node->right) {
        fprintf(fname, "(");
        out_tree_tex(cur_node->right, fname);
        //????
        fprintf(fname, ")");
    }
    else 
        out_tree_tex(cur_node->right, fname);
}

void div_op_proc(Node* cur_node, FILE *fname) {
    fprintf(fname, "\\frac{");
    out_tree_tex(cur_node->left, fname);
    fprintf(fname, "}{");
    out_tree_tex(cur_node->right, fname);
    fprintf(fname, "}");
}

void mul_op_proc(Node* cur_node, FILE *fname) {
    out_tree_tex(cur_node->left, fname);
    out_tree_tex(cur_node->right, fname);
}

void add_op_proc(Node* cur_node, FILE *fname) {
    if (OP_IS(MUL, cur_node->parent)) {
        fprintf(fname, "(");
        out_tree_tex(cur_node->left, fname);
        fprintf(fname, "+");
        out_tree_tex(cur_node->right, fname);
        fprintf(fname, ")");
    }
    else {
        out_tree_tex(cur_node->left, fname);
        fprintf(fname, "+");
        out_tree_tex(cur_node->right, fname);
    }
}

void sub_op_proc(Node* cur_node, FILE *fname) {
    if (OP_IS(MUL, cur_node->parent)) {
        fprintf(fname, "(");
        out_tree_tex(cur_node->left, fname);
        fprintf(fname, "-");
        out_tree_tex(cur_node->right, fname);
        fprintf(fname, ")");
    }
    else {
        out_tree_tex(cur_node->left, fname);
        fprintf(fname, "-");
        out_tree_tex(cur_node->right, fname);
    }
}

void out_tree_tex(Node *cur_node, FILE *fname) {

    if (IS_UN(cur_node))
        un_op_proc(cur_node, fname);

    else if (OP_IS(POW, cur_node))
        pow_op_proc(cur_node, fname);

    else if (OP_IS(LOG, cur_node))
        log_op_proc(cur_node, fname);

    else if (OP_IS(DIV, cur_node))
        div_op_proc(cur_node, fname);

    else if (OP_IS(MUL, cur_node))
        mul_op_proc(cur_node, fname);
    
    else if (OP_IS(ADD, cur_node))
        add_op_proc(cur_node, fname);

    else if (OP_IS(SUB, cur_node))
        sub_op_proc(cur_node, fname);

    else if (TYPE_IS(VAR, cur_node)) 
        fprintf(fname, "%c", var_list[cur_node->v]);

    else if (TYPE_IS(CONST, cur_node)) 
        fprintf(fname, "%d", cur_node->v);
}

Node *copy_node(const Node *node) {
    if (node == nullptr) 
        return nullptr;
    return create_node(node->type, node->v, 
                    copy_tree(node->left), copy_tree(node->right));
}

//L (node->left)
//R (node->right)
//dL (diff_node(L, var))
//dR (diff_node(R, var))
//cL (copy_node(L))
//cR (copy_node(R))
//CONST(C) (create_node(TYPE_CONST, C, nullptr, nullptr))

//++++ overloads:
Node *operator+(Node *lh, Node *rh) {
    return create_node(TYPE_OP, OP_ADD, lh, rh);
}
Node *operator*(Node *lh, Node *rh) {
    return create_node(TYPE_OP, OP_MUL, lh, rh);
}
Node *operator-(Node *lh, Node *rh) {
    return create_node(TYPE_OP, OP_SUB, lh, rh);
}
Node *operator/(Node *lh, Node *rh) {
    return create_node(TYPE_OP, OP_DIV, lh, rh);
}
Node *operator^(Node *lh, Node *rh) {
    return create_node(TYPE_OP, OP_POW, lh, rh);
}
Node *cos(Node *node) {
    return create_node(TYPE_OP, OP_COS, nullptr, node);
}
Node *sin(Node *node) {
    return create_node(TYPE_OP, OP_SIN, nullptr, node);
}
Node *tg(Node *node) {
    return create_node(TYPE_OP, OP_TG, nullptr, node);
}
Node *exp(Node *node) {
    return create_node(TYPE_OP, OP_EXP, nullptr, node);
}
Node *log(Node *node) {
    return create_node(TYPE_OP, OP_LOG, nullptr, node);
}

Node *diff_node(const Node *node, const char var) {
    switch (node->type) {
        case TYPE_CONST:
            return CONST(0);
        case TYPE_VAR:
            //if it is var which we are diff-ing return 1 else 0
            return (node->value == var) ? CONST(1) : CONST(0);
        case TYPE_OP:
            switch (node->value) {
                case OP_ADD:
                    return dL + dR;
                case OP_SUB:
                    return dL - dR;
                case OP_MUL:
                    return dL * cR + cL * dR; //ADD(MUL(dL, cR), MUL(cL, dR));
                case OP_DIV: 
                    return (dL * cR  - cL * dR) / (cR * cR);//DIV(SUB(MUL(dL, cR) MUL(cL, dR)), MUL(cR, cR));
                case OP_SIN:
                    return cos(cR) * dR; //MUL(COS(cR), dR);
                case OP_COS:
                    return CONST(-1) * sin(cR) * dR;//MUL(MUL(SIN(cR), CONST(-1)), dR);
                case OP_EXP:
                    return exp(cR) * dR;//MUL(EXP(cR), dR);
                case OP_TG:
                    return CONST(1) / (cos(cR) * cos(cR)) * dR;//MUL(DIV(CONST(1), MUL(COS(cR), COS(cR))), dR);
                    //it is LN actually
                case OP_LOG:
                    return CONST(1) / cR * dR;//MUL(DIV(CONST(1), cR), dR);
                case OP_POW:
                    //if has no var in its power, calculate as power function
                    if (!find_var(node->right, var))
                        return cR * (cL ^ (cR - CONST(1))) * dL;
                    //if has in power but has not in basement, calculate as exponential
                    else if (!find_var(node->left, var))
                        return log(cL) * dR * (cL ^ cR);
                    else 
                        //calculates derivative of f(x)^g(x) - power-exponential f.
                        return cR * (cL ^ (cR - CONST(1))) * dL + log(cL) * dR * (cL ^ cR);//ADD(MUL(MUL(POW(cL, SUB(cR, CONST(1))), cR), dL), MUL(MUL(POW(cL, cR), LOG(cL)), dR)); 
                default: 
                    return nullptr;
            }
        default:
            return nullptr;
    }
}

bool find_var(Node *node, char var) {
    if (node == nullptr) 
        return 0;
    if (node->type == TYPE_OP) 
        return max(find_var(node->left, var), find_var(node->right, var));
    if (node->type == TYPE_CONST)
        return 0;
    else 
        return (node->v == var) ? 1 : 0;
}

//check if N node is const and N->value equal C.
//CONST_IS(N, C) (N->type == TYPE_CONST && N->value == C)
//IS_ADD()
//ntrl - neutral
int delete_and_substitute(Node *del_node, Node *sub_node, int ntrl_elem) {
    if (CONST_IS(del_node, ntrl_elem)) {
        delete_node(del_node);
        //?????
        sub_node->parent = sub_node;
        return 1;
    }
    else 
        return 0;
}

void in_recursion(Node *node) {
    if (IS_OP(node->left)
       process_neutral(node->left); 
    if (IS_OP(node->right)
       process_neutral(node->right);
}

void change_to_const(Node *node, double val) {
    delete_node(node->left);
    delete_node(node->right);
    node->type = TYPE_CONST;
    node->val = 0;
}
void process_neutral(Node **node) {
    if (*node == nullptr)
        return;
    if (node->type == TYPE_VAR || node->type == TYPE_CONST)
        return;
    else {
        in_recursion(node);
        //if cur node is +/-
        if (IS(ADD, *node) || IS(SUB, *node)) {
            //if its children are operations - go down recursively
            //if left node wasnt deleted (neutral elem wasnt found there respectively)
            // try to find neutral elem in right node
            if (delete_and_substitute(&((*node)->left), &((*node)->right), NEUTRAL_NULL) == 0) 
                delete_and_substitute(&((*node)->right), &((*node)->left), NEUTRAL_NULL); 
            return;
        }
        else if (IS(MUL, *node)) {
            //if left or right argument is 0, fold the whole subtree to 0 const.
            if (CONST_IS((*node)->left, 0) || CONST_IS((*node)->right, 0)) 
                change_to_const(node, 0);
            if (delete_and_substitute(&((*node)->left), &((*node)->right), NEUTRAL_ONE) == 0) 
                delete_and_substitute(&((*node)->right), &((*node)->left), NEUTRAL_ONE); 
            return;
        }
        else if (IS(DIV, *node)) {
            if (CONST_IS((*node)->left, 0)) 
                change_to_const(node, 0);
            //look for neutral ONE in denomenator
            delete_and_substitute(&((*node)->right), &((*node)->left), NEUTRAL_ONE);
        }
        else if (IS(POW, *node)) {
            if (CONST_IS((*node)->left, 1)) 
                change_to_const(node, 1);
            delete_and_substitute(&((*node)->right), &((*node)->left), NEUTRAL_ONE);
        }
}

//The function not values but pointers because it's possible a case
//when lnode will be null ptr(if operation is unary). 
double do_operation(Node *lnode, Node *rnode, int value) {
    swtich (value) {
        case OP_ADD:
            return lnode->value + rnode->value;
        case OP_SUB:
            return lnode->value - rnode->value;
        case OP_MUL:
            return lnode->value * rnode->value;
        case OP_DIV:
            if (abs(rnode->value) > EPS)
                return lnode->value / rnode->value;
            else {
                printf("Emergency exit: dividing by 0\n");
                exit(1);
            }
        case OP_POW:
            return lnode->value ^ rnode->value;
        case OP_SIN:
            return sin(rnode->value);
        case OP_COS:
            return cos(rnode->value);
        case OP_TG:
            return tg(rnode->value);
        case OP_LOG:
            return log(rnode->value);
        case OP_EXP:
            return exp(rnode->value);
        default:
            return 0;
    }
}



bool simplify_const(Node *node) {
    //1 is a neutral element for && operation.
    if (node == nullptr) 
        return 1;
    if (node->type == TYPE_OP) {
        //if both child are const, convert ar. expression in const.
        bool flg = simplify_const(node->left) && simplify_const(node->right);
        if (flg == 1) {
            //changing current node. TYPE_OP turns into TYPE_CONST
            node->type = TYPE_CONST;
            //value gets a result of operation
            double new_value = do_operation(node->left, node->right, node->value);
            node->value = new_value;
            //delete children
            delete_node(node->left);
            delete_node(node->right);
            return flg;
        }
        else
            return flg;
    }
    if (node->type == TYPE_VAR)
        return 0;
    if (node->type == TYPE_CONST)
        return 1;
    else 
        return 0;
}

void cut_tree(Node) {
    
}

///////////////////////////////////////////////////
///////////////////////////////////////////////////
///////////////////////////////////////////////////
void graphvizTree(Node* root)
{
    if (root == nullptr) 
    {
        return;
    }
    FILE* file = fopen("graphPlot.txt", "w");

    fprintf(file, "digraph G{\n");
    fprintf(file, "node [shape=\"record\", style=\"solid\", color=\"green\"];\n");

    loadNudes(file, root);

    fprintf(file, "}");

    fclose(file);

    system("dot -Tjpg  graphPlot.txt -o Graph.jpg");
}

void Graph(Node* root)
{
    printf("Loading graph...\n");
    graphvizTree(root);
    printf("Graph was loaded successful!\n");
}

Node* loadNudes(FILE* file, Node* node)
{
    if (node == nullptr)
    {
        return nullptr;
    }
    if (node->peek_lson() != nullptr)
    {
        fprintf(file, "edge[color=orange]\n\"%p\"->\"%p\";\n", node, node->peek_lson());  // link parent and left child
        loadNudes(file, node->peek_lson());
    }
    if (node->peek_rson() != nullptr)
    {
        fprintf(file, "edge[color=red]\n\"%p\"->\"%p\";\n", node, node->peek_rson()); //link parent and right child
        loadNudes(file, node->peek_rson());
    }
        fprintf(file, "\"%p\" [label=\"{{str: %s}}\"]\n ", node,
            node->peek_data()); // <-- label printing

    return node;
}*/

int main() {
    char *buf = read_file("eq.txt");
    while (*buf != '\0')
        printf("%c", *(buf++));
    return 0;
}