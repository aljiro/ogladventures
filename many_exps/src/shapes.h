#ifndef SHAPES_H
#define SHAPES_H

#include <vector>
#include <armadillo>

using namespace std;

enum ShapeNames{ Cube, Pyramid, Sphere };

class Shapes{
public:

static void cube(vector<arma::vec> *vertices, vector<arma::vec> *faces){
    if( vertices != NULL ){
        (*vertices) = {
            {0.0, 0.0, 0.0},
            {0.0, 0.0, 1.0},
            {0.0, 1.0, 1.0},
            {0.0, 1.0, 0.0},
            {1.0, 1.0, 0.0},
            {1.0, 0.0, 0.0},
            {1.0, 0.0, 1.0},
            {1.0, 1.0, 1.0}
        };
    }

    if( faces != NULL ){
        (*faces) = {{0, 1, 2}, {0, 3, 2}, {0, 3, 4}, {0, 5, 4}, 
                    {5, 4, 7}, {5, 6, 7}, {1, 2, 7}, {1, 6, 7},
                    {2, 3, 4}, {2, 7, 4}, {1, 2, 7}, {1, 6, 7}};
    }
}
};

#endif