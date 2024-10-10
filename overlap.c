#include <stdio.h>
#include <stdlib.h>

//struct definitions
typedef struct { //struct for the points in points.txt
    int x;
    int y;
} Point;

typedef struct { //struct for user inputed points
    int center_x;
    int center_y;
    int radius_squared;
} Circle;

Point* points = NULL;
int num_points = 0;

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

//function to check if a point collides with the circle
int is_collision(const Circle* circ, const Point* point) {
    int dx = circ->center_x - point->x;
    int dy = circ->center_y - point->y;
    return dx*dx + dy*dy <= circ->radius_squared;
}

// Binary search to find the leftmost point that might collide
int binary_search_left(const Circle* circ, int low, int high) {
    while (low < high) {
        int mid = low + (high - low) / 2;
        if (points[mid].x >= circ->center_x - circ->radius_squared)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

// Binary search to find the rightmost point that might collide
int binary_search_right(const Circle* circ, int low, int high) {
    while (low < high) {
        int mid = low + (high - low + 1) / 2;
        if (points[mid].x <= circ->center_x + circ->radius_squared)
            low = mid;
        else
            high = mid - 1;
    }
    return high;
}

//function to count the number of collisions
int count_collisions(const Circle* circ) {
    int left = binary_search_left(circ, 0, num_points - 1);
    int right = binary_search_right(circ, left, num_points - 1);
    
    int collisions = 0;
    for (int i = left; i <= right; i++) {
        if (is_collision(circ, &points[i])) {
            collisions++;
        }
    }
    return collisions;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    read_points(argv[1]);

    // Sorting points
    qsort(points, num_points, sizeof(Point), compare_points);

    Circle circ;
    int radius;
    while (scanf("%d %d %d", &circ.center_x, &circ.center_y, &radius) == 3) {
        circ.radius_squared = radius * radius;
        printf("%d\n", count_collisions(&circ));
    }

    free(points);
    return 0;
}