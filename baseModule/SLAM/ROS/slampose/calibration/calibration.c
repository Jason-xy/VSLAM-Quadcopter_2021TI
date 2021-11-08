#include <stdio.h>
#include <math.h>

int main(int argc, char **argv){
    if(argc != 5){
        printf("Usage: ./calibration x1 y1 x1 y2");
        return 1;
    }

    float x1 = atof(argv[1]), y1 = atof(argv[2]), x2 = atof(argv[3]), y2 = atof(argv[4]);
    float theta_1 = atan(x1 / y1);
    float theta_2 = atan(y2 / x2);
    float theta = 0;
    if(x1 < 0)
        theta = (theta_1 - theta_2) / 2;
    else 
        theta = (theta_1 - theta_2) / 2;
    float scale_x = sqrt(x2 * x2 + y2 * y2) / 300;
    float scale_y = sqrt(x1 * x1 + y1 * y1) / 350;
    printf("[Theta]: %f\n[scale_x]: %f\n[scale_y]: %f\n", theta, scale_x, scale_y);
    return 0;
}