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
    int radius_squared;  // Store radius squared to avoid sqrt in distance calculation (first version timed out)
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

//function to check if a point collides with the circle
int is_collision(Circle* circ, Point* point) {
    int dx = circ->center_x - point->x;
    int dy = circ->center_y - point->y;
    return dx*dx + dy*dy <= circ->radius_squared;
}

//function to count the number of collisions
int count_collisions(Circle* circ) {
    int collisions = 0;
    for (int i = 0; i < num_points; i++) {
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

    Circle circ;
    int radius;
    while (scanf("%d %d %d", &circ.center_x, &circ.center_y, &radius) == 3) {
        circ.radius_squared = radius * radius;
        printf("%d\n", count_collisions(&circ));
    }

    free(points);
    return 0;
}