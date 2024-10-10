#include <stdio.h>
#include <stdlib.h>

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
    int center_x;
    int center_y;
    int radius_squared;
} Circle;

// Global variables
TreeNode* root = NULL;
int num_points = 0;

// Function to get max of two integers
int max(int a, int b) {
    return (a > b) ? a : b;
}

// Function to get height of the tree
int height(TreeNode *N) {
    if (N == NULL)
        return 0;
    return N->height;
}

// Function to create a new node
TreeNode* newNode(Point point) {
    TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));
    node->point = point;
    node->left = NULL;
    node->right = NULL;
    node->height = 1;
    return(node);
}

// Function to right rotate subtree rooted with y
TreeNode *rightRotate(TreeNode *y) {
    TreeNode *x = y->left;
    TreeNode *T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(height(y->left), height(y->right))+1;
    x->height = max(height(x->left), height(x->right))+1;

    return x;
}

// Function to left rotate subtree rooted with x
TreeNode *leftRotate(TreeNode *x) {
    TreeNode *y = x->right;
    TreeNode *T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(height(x->left), height(x->right))+1;
    y->height = max(height(y->left), height(y->right))+1;

    return y;
}

// Get Balance factor of node N
int getBalance(TreeNode *N) {
    if (N == NULL)
        return 0;
    return height(N->left) - height(N->right);
}

// Function to insert a node
TreeNode* insert(TreeNode* node, Point point) {
    if (node == NULL)
        return(newNode(point));

    if (point.x < node->point.x)
        node->left = insert(node->left, point);
    else if (point.x > node->point.x)
        node->right = insert(node->right, point);
    else {
        if (point.y < node->point.y)
            node->left = insert(node->left, point);
        else
            node->right = insert(node->right, point);
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
        exit(1);
    }

    int x, y;
    while (fscanf(file, "%d %d", &x, &y) == 2) {
        Point point = {x, y};
        root = insert(root, point);
        num_points++;
    }

    fclose(file);
}

// Function to check if a point collides with the circle
int is_collision(const Circle* circ, const Point* point) {
    long long dx = (long long)circ->center_x - point->x;
    long long dy = (long long)circ->center_y - point->y;
    return dx*dx + dy*dy <= (long long)circ->radius_squared;
}

// Function to count collisions
int count_collisions(TreeNode* node, const Circle* circ) {
    if (node == NULL)
        return 0;

    int count = 0;
    if (is_collision(circ, &node->point))
        count = 1;

    // If circle is to the right of the node
    if (circ->center_x - circ->radius_squared > node->point.x)
        return count + count_collisions(node->right, circ);

    // If circle is to the left of the node
    if (circ->center_x + circ->radius_squared < node->point.x)
        return count + count_collisions(node->left, circ);

    // Circle overlaps, need to check both subtrees
    return count + count_collisions(node->left, circ) + count_collisions(node->right, circ);
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
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    read_points(argv[1]);

    Circle circ;
    int radius;
    while (scanf("%d %d %d", &circ.center_x, &circ.center_y, &radius) == 3) {
        circ.radius_squared = (long long)radius * radius;
        printf("%d\n", count_collisions(root, &circ));
    }

    free_tree(root);
    return 0;
}