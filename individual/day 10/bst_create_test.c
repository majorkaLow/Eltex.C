#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bst.h"

int main() {
    node* root = NULL;
    root = create(root, 17);
    root = bstree_create_node(root, 18);
    root = bstree_create_node(root, 10);

    printf("\t%d\n", root->key);

    if (root->left == NULL) {
        printf("Узла нет\t%d\n", root->right->key);
    } else if (root->right == NULL) {
        printf("%d\t Узла нет\n", root->left->key);
    } else {
        printf("%d\t\t%d\n", root->left->key, root->right->key);
    }

    free(root->left);
    free(root->right);
    free(root);

    return 0;
}