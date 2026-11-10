/* HSTMATCH module=EBOOT.BIN start=0x0006c934 end=0x0006cc00 symbol=FUN_0006c934 lang=c++ */
/* Clone of src/sharing_main/tree_destroy_0006df1c.cpp (hstclone.py tag c145620): addresses substituted; exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly (recursion levels spelled out as in src/eboot/tree_destroy_000d71a8.cpp; the inner levels call the out-of-line accessors 0x6e23c/0x6e210, allocator destroy 0x6e218, deallocate 0x6e208 and value dtor 0x69ea8); exact match verified (matching idiom: the top-level value teardown spells the nested inline-dtor layers as repeated null tests of the member addresses, as in FUN_00068a54.c); names and types await human review. */
#pragma inline_depth(8)
struct Obj36 { int a, b; };
struct Vec24 { char *data; int size; int cap; };
struct Second { int f0, f4; Vec24 vec; Obj36 obj; };
struct Value { int key; Second second; };
struct Node { Node *left; Node *right; Node *parent; Value value; };
struct Tree;
extern "C" {
void *FUN_0006cc54(Tree *tree);
void FUN_0006cc30(void *a, Value *p);
void *FUN_0006cc28(Tree *tree);
void FUN_0006cc20(void *a, Node *p, unsigned n);
void FUN_00068a54(Value *p, int flags);
void FUN_0006b96c(Obj36 *p, int flags);
void FUN_0006cc10(Vec24 *v, int n);
void *FUN_0006cc08(Vec24 *v);
void *FUN_0006cc00(Vec24 *v);
void FUN_0006c934(Tree *tree, Node *node);
}
static inline void Destroy2(Tree *tree, Node *node)
{
    if (node->left)
        FUN_0006c934(tree, node->left);
    if (node->right)
        FUN_0006c934(tree, node->right);
    FUN_0006cc30(FUN_0006cc54(tree), &node->value);
    FUN_0006cc20(FUN_0006cc28(tree), node, 1);
}
static inline void Destroy1(Tree *tree, Node *node)
{
    if (node->left)
        Destroy2(tree, node->left);
    if (node->right)
        Destroy2(tree, node->right);
    FUN_0006cc54(tree);
    FUN_00068a54(&node->value, -1);
    FUN_0006cc28(tree);
    operator delete(node);
}
static inline void DestroyValue(Value *v)
{
    if (v != 0) {
        if (&v->second != 0) {
            if (&v->second.obj != 0 && &v->second.obj != 0 && &v->second.obj != 0 && &v->second.obj != 0)
                FUN_0006b96c(&v->second.obj, 0);
            if (&v->second.vec != 0 && &v->second.vec != 0 && &v->second.vec != 0) {
                FUN_0006cc10(&v->second.vec, v->second.vec.size);
                if (v->second.vec.data != 0) {
                    FUN_0006cc08(&v->second.vec);
                    char *p = v->second.vec.data;
                    FUN_0006cc00(&v->second.vec);
                    operator delete(p);
                }
            }
        }
    }
}
extern "C" void FUN_0006c934(Tree *tree, Node *node)
{
    if (node->left)
        Destroy1(tree, node->left);
    if (node->right)
        Destroy1(tree, node->right);
    DestroyValue(&node->value);
    operator delete(node);
}
