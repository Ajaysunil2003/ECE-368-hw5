#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//function to take the input file and it will also take the user input (function called inputs)
    //Take the user inputed point and store it properly (This is the dimensions of the circle)
        //call it circ_dim (first numner is the x coord of center, second is y coord of center, third number is radius)

    //first take the input ile (points.txt)
        //store it properly (This points.txt file is a set of points that will be tested with circ_dim)

//function to iterate through the points in points.txt file (iteration)
    //iterate through each point one by one to be calculated in edistance formula function

//distance formula function (dst_form)
    //i = 0 //this is a counter for the number of points less than radius
    //use the distance formula to calculate wheather the distance of the current point in points.txt is less than or greater than the radius
    //if it is less than the radius than:
        //i++
    //if it isnt less than the radius than iterate to the next point in points.txt and do the same thing

    //print i


//main:
    //call inputs function
    //call iteration function
    //call dst_form function
    //return 0

typedef struct {
    int x;
    int y;
} Point;

typedef struct {
    int center_x;
    int center_y;
    int radius;
} Circle;

Point* points = NULL;
int num_points = 0;

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

double calculate_distance(int x1, int y1, int x2, int y2) {
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

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