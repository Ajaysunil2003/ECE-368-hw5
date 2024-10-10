#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

// Struct definitions
typedef struct {
    int x;
    int y;
} Point;

typedef struct TreeNode {
    Point point;
    struct TreeNode *left, *right;
    int height;
} TreeNode;

typedef struct {
    long long center_x;
    long long center_y;
    long long radius_squared;
} Circle;

// Global variables
TreeNode* root = NULL;

// Function to get max of two integers
int max(int a, int b) {
    return (a > b) ? a : b;
}

// Function to get height of the tree
int height(TreeNode *N) {
    return N ? N->height : 0;
}

// Function to create a new node
TreeNode* newNode(Point point) {
    TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));
    if (node == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    node->point = point;
    node->left = node->right = NULL;
    node->height = 1;
    return node;
}

// Function to right rotate subtree rooted with y
TreeNode *rightRotate(TreeNode *y) {
    TreeNode *x = y->left;
    TreeNode *T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    return x;
}

// Function to left rotate subtree rooted with x
TreeNode *leftRotate(TreeNode *x) {
    TreeNode *y = x->right;
    TreeNode *T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    return y;
}

// Get Balance factor of node N
int getBalance(TreeNode *N) {
    return N ? height(N->left) - height(N->right) : 0;
}

// Function to insert a node
TreeNode* insert(TreeNode* node, Point point) {
    if (node == NULL)
        return newNode(point);

    if (point.x < node->point.x)
        node->left = insert(node->left, point);
    else if (point.x > node->point.x)
        node->right = insert(node->right, point);
    else {
        if (point.y < node->point.y)
            node->left = insert(node->left, point);
        else if (point.y > node->point.y)
            node->right = insert(node->right, point);
        else
            return node;  // Duplicate point, don't insert
    }

    node->height = 1 + max(height(node->left), height(node->right));

    int balance = getBalance(node);

    // Left Left Case
    if (balance > 1 && point.x < node->left->point.x)
        return rightRotate(node);

    // Right Right Case
    if (balance < -1 && point.x > node->right->point.x)
        return leftRotate(node);

    // Left Right Case
    if (balance > 1 && point.x > node->left->point.x) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    // Right Left Case
    if (balance < -1 && point.x < node->right->point.x) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

// Function to read points from file
void read_points(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Could not open file\n");
        exit(1);
    }

    int x, y;
    while (fscanf(file, "%d %d", &x, &y) == 2) {
        Point point = {x, y};
        root = insert(root, point);
    }

    fclose(file);
}

// Function to check if a point collides with the circle
int is_collision(const Circle* circ, const Point* point) {
    long long dx = circ->center_x - point->x;
    long long dy = circ->center_y - point->y;
    return dx*dx + dy*dy <= circ->radius_squared;
}

// Function to count collisions
int count_collisions(TreeNode* node, const Circle* circ, int* count) {
    if (node == NULL)
        return 0;

    if (is_collision(circ, &node->point))
        (*count)++;

    long long left_bound = circ->center_x - (long long)INT_MAX;
    long long right_bound = circ->center_x + (long long)INT_MAX;

    if (node->point.x > right_bound)
        count_collisions(node->left, circ, count);
    else if (node->point.x < left_bound)
        count_collisions(node->right, circ, count);
    else {
        count_collisions(node->left, circ, count);
        count_collisions(node->right, circ, count);
    }

    return *count;
}

// Function to free the AVL tree
void free_tree(TreeNode* node) {
    if (node == NULL)
        return;
    free_tree(node->left);
    free_tree(node->right);
    free(node);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    read_points(argv[1]);

    Circle circ;
    int radius;
    while (scanf("%d %d %d", &radius, (int*)&circ.center_x, (int*)&circ.center_y) == 3) {
        circ.radius_squared = (long long)radius * radius;
        int count = 0;
        printf("%d\n", count_collisions(root, &circ, &count));
    }

    free_tree(root);
    return 0;
}