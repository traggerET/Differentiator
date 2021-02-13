#include <string>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <cstring>
#include <cassert>
#include <cmath>
#include <cstdlib>
using namespace std;
#include "../funcs/functions_decl.hpp"


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