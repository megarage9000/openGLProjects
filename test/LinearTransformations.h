#ifndef LINEAR_TRANSFORMATIONS
#define LINEAR_TRANSFORMATIONS

#include "LinearAlgebra.h"
namespace LinearTransformations {

    // Translations
    void translate(float transformMatrix[], float translationVector[]);

    // Scales
    void scale(float transformMatrix[], float scaleVector[]);

    // Rotations
    void rotateX(float transformMatrix[], float rotationX);
    void rotateY(float transformMatrix[], float rotationY);
    void rotateZ(float transformMatrix[], float rotationZ);

    // View Matrix
    void viewMatrix(float viewMatrix[], float upVector[], float forwardVector[], float camWorldPosition[]);

    // Projection Matrix
    void projectionMatrix(float resultMatrix[], float near, float far, float fov, float range, float aspect);
}
#endif