#ifndef SPHERE_H
#define SPHERE_H

#include "GenericObject.h"

class Sphere : public GenericObject {
public:
    float radius;
    int value;

    Sphere(glm::vec3 pos, glm::vec3 col, float r)
        : GenericObject(pos, col), radius(r) {
    }
    // Constructor for rotating sphere (with orbit parameters)
    // Constructor for a rotating sphere
    Sphere(glm::vec3 pos, glm::vec3 col, float r, int value)
        : GenericObject(pos, col), radius(r) ,value(value){
    }


    void Render(const glm::mat4& projection, const glm::mat4& view) override;
};

#endif
