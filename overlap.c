#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

// Structure to represent a 2D point
typedef struct {
    int x;
    int y;
} Point;

// Structure for a node in the binary search tree
typedef struct TreeNode {
    Point point;
    struct TreeNode *left, *right;
} TreeNode;

// Global root of the tree
TreeNode* root = NULL;

// Function to create a new node
TreeNode* newNode(Point point) {
    TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));
    if (node == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    node->point = point;
    node->left = node->right = NULL;
    return node;
}

// Function to insert a point into the tree
TreeNode* insert(TreeNode* node, Point point) {
    if (node == NULL)
        return newNode(point);

    // Insert based on x-coordinate first, then y-coordinate
    if (point.x < node->point.x)
        node->left = insert(node->left, point);
    else if (point.x > node->point.x)
        node->right = insert(node->right, point);
    else {
        if (point.y < node->point.y)
            node->left = insert(node->left, point);
        else if (point.y > node->point.y)
            node->right = insert(node->right, point);
    }

    return node;
}

// Function to read points from file and build the tree
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

// Function to count collisions
int count_collisions(TreeNode* node, long long cx, long long cy, long long r_squared) {
    if (node == NULL)
        return 0;

    int count = 0;
    long long dx = cx - node->point.x;
    long long dy = cy - node->point.y;
    
    // Check if current point is inside the circle
    if (dx*dx + dy*dy <= r_squared)
        count = 1;

    // Recursively check left subtree if circle extends to the left
    if (cx - r_squared <= node->point.x)
        count += count_collisions(node->left, cx, cy, r_squared);

    // Recursively check right subtree if circle extends to the right
    if (cx + r_squared >= node->point.x)
        count += count_collisions(node->right, cx, cy, r_squared);

    return count;
}

// Function to free the tree
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

    // Read points from file and build the tree
    read_points(argv[1]);

    // Process queries
    int cx, cy, r;
    while (scanf("%d %d %d", &cx, &cy, &r) == 3) {
        long long r_squared = (long long)r * r;
        printf("%d\n", count_collisions(root, cx, cy, r_squared));
    }

    // Free allocated memory
    free_tree(root);
    return 0;
}