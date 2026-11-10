/* HSTMATCH module=hstp_trans_main.prx start=0x0006df1c end=0x0006e1e8 symbol=FUN_0006df1c lang=c++ */
/* Hand-written reconstruction from the disassembly (recursion levels spelled out as in src/eboot/tree_destroy_000d71a8.cpp; the inner levels call the out-of-line accessors 0x6e23c/0x6e210, allocator destroy 0x6e218, deallocate 0x6e208 and value dtor 0x69ea8); exact match verified (matching idiom: the top-level value teardown spells the nested inline-dtor layers as repeated null tests of the member addresses, as in FUN_00069ea8.c); names and types await human review. */
#pragma inline_depth(8)
struct Obj36 { int a, b; };
struct Vec24 { char *data; int size; int cap; };
struct Second { int f0, f4; Vec24 vec; Obj36 obj; };
struct Value { int key; Second second; };
struct Node { Node *left; Node *right; Node *parent; Value value; };
struct Tree;
extern "C" {
void *FUN_0006e23c(Tree *tree);
void FUN_0006e218(void *a, Value *p);
void *FUN_0006e210(Tree *tree);
void FUN_0006e208(void *a, Node *p, unsigned n);
void FUN_00069ea8(Value *p, int flags);
void FUN_0006cf54(Obj36 *p, int flags);
void FUN_0006e1f8(Vec24 *v, int n);
void *FUN_0006e1f0(Vec24 *v);
void *FUN_0006e1e8(Vec24 *v);
void FUN_0006df1c(Tree *tree, Node *node);
}
static inline void Destroy2(Tree *tree, Node *node)
{
    if (node->left)
        FUN_0006df1c(tree, node->left);
    if (node->right)
        FUN_0006df1c(tree, node->right);
    FUN_0006e218(FUN_0006e23c(tree), &node->value);
    FUN_0006e208(FUN_0006e210(tree), node, 1);
}
static inline void Destroy1(Tree *tree, Node *node)
{
    if (node->left)
        Destroy2(tree, node->left);
    if (node->right)
        Destroy2(tree, node->right);
    FUN_0006e23c(tree);
    FUN_00069ea8(&node->value, -1);
    FUN_0006e210(tree);
    operator delete(node);
}
static inline void DestroyValue(Value *v)
{
    if (v != 0) {
        if (&v->second != 0) {
            if (&v->second.obj != 0 && &v->second.obj != 0 && &v->second.obj != 0 && &v->second.obj != 0)
                FUN_0006cf54(&v->second.obj, 0);
            if (&v->second.vec != 0 && &v->second.vec != 0 && &v->second.vec != 0) {
                FUN_0006e1f8(&v->second.vec, v->second.vec.size);
                if (v->second.vec.data != 0) {
                    FUN_0006e1f0(&v->second.vec);
                    char *p = v->second.vec.data;
                    FUN_0006e1e8(&v->second.vec);
                    operator delete(p);
                }
            }
        }
    }
}
extern "C" void FUN_0006df1c(Tree *tree, Node *node)
{
    if (node->left)
        Destroy1(tree, node->left);
    if (node->right)
        Destroy1(tree, node->right);
    DestroyValue(&node->value);
    operator delete(node);
}
