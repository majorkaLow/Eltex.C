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

    printf("\t\t%d\n", root->key);
    printf("\t%d\t\t%d\n", root->left->key, root->right->key);
    printf("%d\t\t%d\n", root->left->left->key, root->left->right->key);

    // Освобождение памяти
    free(root->left->left);
    free(root->left->right);
    free(root->left);
    free(root->right);
    free(root);

    return 0;
}