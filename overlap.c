#include <stdio.h>
#include <stdlib.h>

//struct definitions
typedef struct { //struct for the points in points.txt
    int x;
    int y;
} Point;

typedef struct TreeNode {
    Point point;
    struct TreeNode *left, *right;
} TreeNode;

typedef struct { //struct for user inputed points
    int center_x;
    int center_y;
    int radius_squared;
} Circle;

Point* points = NULL;
int num_points = 0;
TreeNode* root = NULL;

//function to take the input file and it will also take the user input
void read_points(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        exit(1);
    }

    int x, y;
    while (fscanf(file, "%d %d", &x, &y) == 2) {
        num_points++;
        points = realloc(points, num_points * sizeof(Point));
        points[num_points - 1].x = x;
        points[num_points - 1].y = y;
    }

    fclose(file);
}

// Compare function for qsort
int compare_points(const void* a, const void* b) {
    Point* p1 = (Point*)a;
    Point* p2 = (Point*)b;
    if (p1->x != p2->x) return p1->x - p2->x;
    return p1->y - p2->y;
}

// Function to create a new node
TreeNode* new_node(Point point) {
    TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));
    node->point = point;
    node->left = node->right = NULL;
    return node;
}

// Function to create balanced BST from sorted array
TreeNode* sorted_array_to_bst(Point* points, int start, int end) {
    if (start > end) return NULL;
    
    int mid = (start + end) / 2;
    TreeNode* node = new_node(points[mid]);
    
    node->left = sorted_array_to_bst(points, start, mid - 1);
    node->right = sorted_array_to_bst(points, mid + 1, end);
    
    return node;
}

//function to check if a point collides with the circle
int is_collision(Circle* circ, Point* point) {
    int dx = circ->center_x - point->x;
    int dy = circ->center_y - point->y;
    return dx*dx + dy*dy <= circ->radius_squared;
}

//function to count the number of collisions using BST
int count_collisions_bst(TreeNode* node, Circle* circ) {
    if (node == NULL) return 0;
    
    int count = 0;
    if (is_collision(circ, &node->point)) {
        count = 1;
    }
    
    // If the circle's leftmost x-coordinate is to the right of the node's x-coordinate,
    //don't need to check the left subtree
    if (circ->center_x - circ->radius_squared <= node->point.x) {
        count += count_collisions_bst(node->left, circ);
    }
    
    // If the circle's rightmost x-coordinate is to the left of the node's x-coordinate,
    //don't need to check the right subtree
    if (circ->center_x + circ->radius_squared >= node->point.x) {
        count += count_collisions_bst(node->right, circ);
    }
    
    return count;
}

// Function to free the BST
void free_bst(TreeNode* node) {
    if (node == NULL) return;
    free_bst(node->left);
    free_bst(node->right);
    free(node);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    read_points(argv[1]);

    // Sort the points
    qsort(points, num_points, sizeof(Point), compare_points);

    // Create balanced BST
    root = sorted_array_to_bst(points, 0, num_points - 1);

    Circle circ;
    int radius;
    while (scanf("%d %d %d", &circ.center_x, &circ.center_y, &radius) == 3) {
        circ.radius_squared = radius * radius;
        printf("%d\n", count_collisions_bst(root, &circ));
    }

    
    free_bst(root);
    free(points);
    return 0;
}