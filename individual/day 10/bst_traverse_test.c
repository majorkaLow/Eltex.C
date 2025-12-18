#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bst.h"

int main() {
    node* root = NULL;
    root = create(root, 17);
    bstree_insert(&root, 13, compare_ints);
    bstree_insert(&root, 18, compare_ints);
    bstree_insert(&root, 12, compare_ints);
    bstree_insert(&root, 14, compare_ints);

    printf("bstree_apply_infix\n");
    bstree_apply_infix(root, print_key);

    printf("bstree_apply_prefix\n");
    bstree_apply_prefix(root, print_key);

    printf("bstree_apply_postfix\n");
    bstree_apply_postfix(root, print_key);
    free(root->left->left);
    free(root->left->right);
    free(root->left);
    free(root->right);
    free(root);

    return 0;
}