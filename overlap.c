#include <stdio.h>
#include <stdlib.h>
#include <math.h>
//struct definitions
typedef struct { //struct for the points in points.txt
    int x;
    int y;
} Point;

typedef struct { //struct for user inputed points
    int center_x;
    int center_y;
    int radius;
} Circle;

Point* points = NULL;
int num_points = 0;

//function to take the input file and it will also take the user input
void read_points(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file\n");
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
//distance formula function
double calculate_distance(int x1, int y1, int x2, int y2) {
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}
//function to count the number of collisions
int count_collisions(Circle circ) {
    int collisions = 0;
    for (int i = 0; i < num_points; i++) {
        double distance = calculate_distance(circ.center_x, circ.center_y, points[i].x, points[i].y);
        if (distance <= circ.radius) {
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
    while (scanf("%d %d %d", &circ.center_x, &circ.center_y, &circ.radius) == 3) {
        int collisions = count_collisions(circ);
        printf("%d\n", collisions);
    }

    free(points);
    return 0;
}