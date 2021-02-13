
#include <string>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <cstring>

using namespace std;
#define IS(OPERATION, node) ((node != nullptr) && (node->type == TYPE_OP && node->v == OP_ ## OPERATION))
#define IS_UN(node) (node != nullptr && node->type == TYPE_OP && (node->v == OP_SIN || node->v == OP_cos ||

#define pech (printf("fuck"))

const char var_list[] = {'a', 'b', 'c', 'd', 'e',
                        'f', 'g', 'h', 'i', 'k', 
                        'l', 'm', 'n', 'o', 'p', 
                        'r', 's', 't', 'u', 'v',
                        'w', 'x', 'y', 'z'};


const int TYPE_OP = 1;
const int OP_MUL = 10;
const int OP_ADD = 9;
const int OP_SUB = 8;
const int OP_DIV = 7;

#define T (node->type)
struct  Node{
    int type;
    int v;
    int* ptr;

};


int main() {
    //Node *a = new Node(1, 10);
    //if IS(ADD, a)
    Node a;
    if (a.ptr == nullptr)
        cout << "yes";
    else 
        cout << "no";
    return 0;
}