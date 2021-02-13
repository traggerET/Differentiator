class Node {
    public:
    virtual ~Node();

    virtual Node *clone() = 0;
    virtual void copy(Node *c_node);

    Node *getRch();
    Node **getRchptr();
    Node *getLch();
    Node **getLchptr();
    void setLeft(Node *ptr);
    void setRight(Node *ptr);
    unsigned int getType();
    virtual double getVal() = 0;
    //differentiation function
    virtual Node *d(unsigned int var) = 0;

    protected:
    //Node *parent;
    Node *left;
    Node *right;
    unsigned type;
};

class OpNode : public Node {
    private: 
    unsigned int val;
    
    public:
    OpNode(unsigned int type_op = 0, unsigned int op_num = 0, Node *lptr = nullptr, Node *rptr = nullptr);
    virtual ~OpNode();
    virtual Node *clone();
    virtual void copy(Node *c_node);

    virtual double getVal(); 
    const char *getUnOpName();
    //differentiation function
    virtual Node *d(unsigned int dif_val);
};

class ConstNode : public Node {
    private:
    double val;

    public:    
    ConstNode(unsigned int type_const = 0, double new_val = 0, Node *lptr = nullptr, Node *rptr = nullptr);
    virtual ~ConstNode();
    virtual Node *clone();
    virtual void copy(Node *c_node);

    virtual double getVal();
    virtual Node *d(unsigned int dif_val);

};

class VarNode : public Node {
    private:
    unsigned int val;

    public: 
    VarNode(unsigned int type_var = 0, unsigned int new_val = 0, Node *lptr = nullptr, Node *rptr = nullptr);
    virtual ~VarNode();
    virtual Node *clone();
    virtual void copy(Node *c_node);

    virtual double getVal(); 
    virtual Node *d(unsigned int dif_val);
};

class Tree {
    private:
    Node *root;
    Node* parse_eq(char** cur_pos);
    //returns true if there has been made simplifying in the tree; false otherwise
    bool simplify_const(Node **node);

    bool process_neutral(Node **node);
    bool mul_ntrl_process(Node **node);
    bool add_ntrl_process(Node **node);
    bool sub_ntrl_process(Node **node);
    bool div_ntrl_process(Node **node);
    bool pow_ntrl_process(Node **node);
    void hangtoLeft(Node **node);
    void hangtoRight(Node **node);
    protected:

    public:
    //constructs empty tree if fbegin_ == nullptr, if not - reads from buff and create a real tree 
    Tree(char **fbegin_ = nullptr);
    ~Tree();

    Node *getRoot();
    void setRoot(Node *ptr);
    Node **getRootptr();

    Node *diff_by_var(unsigned int var);
    //simplifies tree until it's perfect
    void TakeCare_of_Tree();
};