
#include <string>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <cstring>
#include <cassert>
#include <cmath>
#include <cstdlib>
//#include "Class_declaring.hpp"
#include "../funcs/functions_decl.hpp"
char var_list[26];
Node::~Node() {}

void Node::copy(Node *c_node) {
    if (c_node->left != nullptr) 
        left = c_node->left->clone();
    else 
        left = nullptr;
    if (c_node->right != nullptr) 
        right = c_node->right->clone();
    else
        right = nullptr;
    type = c_node->type;
    //parent = c_node->parent;
}

Node *Node::getRch() {
    return right;
}
Node **Node::getRchptr() {
    return &right;
}
Node *Node::getLch() {
    return left;
}
Node **Node::getLchptr() {
    return &left;
}
void Node::setLeft(Node *ptr) {
    left = ptr;
}
void Node::setRight(Node *ptr) {
    right = ptr;
}

unsigned Node::getType() {
    return type;
}


OpNode::OpNode(unsigned type_op, unsigned op_num, Node *lptr, Node *rptr) {
    type = type_op;
    val = op_num;
    left = lptr;
    right = rptr;
}
OpNode::~OpNode() {
    delete right;
    delete left;
}

Node *OpNode::clone() {
    OpNode *node = new OpNode();
    node->copy(this);
    return node;
}
void OpNode::copy(Node *c_node) {
    Node::copy(c_node);
    val = ((OpNode*)c_node)->val;
}


double OpNode::getVal() {
    return (double)val;
}   
const char *OpNode::getUnOpName() {
    switch (val) {
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

#define dL (*(left->d(dif_val)))
#define dR (*(right->d(dif_val)))
#define cL (*(left->clone()))
#define cR (*(right->clone()))
    //differentiation function
Node *OpNode::d(unsigned dif_val) {
    switch (val) {
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

   
ConstNode::ConstNode(unsigned type_const, double new_val, Node *lptr, Node *rptr){
    type = type_const;
    val = new_val;
    left = lptr;
    right = rptr;
}
ConstNode::~ConstNode() {
    delete right;
    delete left;
}

Node *ConstNode::clone() {
    ConstNode *node = new ConstNode();
    node->copy(this);
    return node;
}
void ConstNode::copy(Node *c_node) {
    Node::copy(c_node);
    val = ((ConstNode*)c_node)->val;
}

double ConstNode::getVal() {
    return (double)val;
}   

Node *ConstNode::d(unsigned dif_val) {
    return new ConstNode(TYPE_CONST, 0, nullptr, nullptr);
}
 

VarNode::VarNode(unsigned type_var, unsigned new_val, Node *lptr, Node *rptr) {
    val = new_val;
    type = type_var;
    left = lptr;
    right = rptr;
}
VarNode::~VarNode() {
    delete right;
    delete left;
}

Node *VarNode::clone() {
    VarNode *node = new VarNode();
    node->copy(this);
    return node;
}
void VarNode::copy(Node *c_node) {
    Node::copy(c_node);
    val = ((VarNode*)c_node)->val;
}

double VarNode::getVal() {
    return (double)val;
}   

Node *VarNode::d(unsigned dif_val) {
    if (val == dif_val)
        return new ConstNode(TYPE_CONST, 1, nullptr, nullptr);
    else 
        return new ConstNode(TYPE_CONST, 0, nullptr, nullptr);
}


//constructs empty tree if fbegin_ == nullptr, if not - reads from buff and create a real tree 
Tree::Tree(char **fbegin_) {
        if (fbegin_ != nullptr)
            root = parse_eq(fbegin_);
    }
Node *Tree::parse_eq(char** cur_pos) {
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
            return new OpNode(TYPE_OP, op_code, l_node, r_node);
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
            return new VarNode(TYPE_VAR, pos_in_arr, nullptr, nullptr);
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
            Node *tmp_node = new OpNode(TYPE_OP, op_code, nullptr, parse_eq(cur_pos));
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
bool Tree::simplify_const(Node **node) {
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

bool Tree::mul_ntrl_process(Node **node) {
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
bool Tree::add_ntrl_process(Node **node) {
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
bool Tree::sub_ntrl_process(Node **node) {
    if (is_null((*node)->getRch())) {
        hangtoLeft(node);
        return true;
    }
    else 
        return false;
}
bool Tree::div_ntrl_process(Node **node) {
    if (is_one((*node)->getRch())) {
        hangtoLeft(node);
        return true;
    }
    else 
        return false;
}
bool Tree::pow_ntrl_process(Node **node) {
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

void Tree::hangtoLeft(Node **node) {
    Node* tmpnode = (*node)->getLch();
    (*node)->setLeft(nullptr);
    delete *node;
    *node = tmpnode;
}
void Tree::hangtoRight(Node **node) {
    Node* tmpnode = (*node)->getRch();
    (*node)->setRight(nullptr);
    delete *node;
    *node = tmpnode; 
}
bool Tree::process_neutral(Node **node) {
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
    

Node *Tree::diff_by_var(unsigned int var) {
    return root->d(var);
}
    //simplifies tree until it's perfect
void Tree::TakeCare_of_Tree() {
    while (simplify_const(&root) || process_neutral(&root));
}

Node *Tree::getRoot() {
    return root;
}
void Tree::setRoot(Node *ptr) {
    root = ptr;
}
Node **Tree::getRootptr() {
    return &root;
}
Tree::~Tree() {
    delete root;
}