#ifndef CUBOID_H
#define CUBOID_H

#include "GenericObject.h"

class Cuboid : public GenericObject {
public:
    glm::vec3 size;

    Cuboid(glm::vec3 pos, glm::vec3 col, glm::vec3 s)
        : GenericObject(pos, col), size(s) {
    }

    void Render(const glm::mat4& projection, const glm::mat4& view) override;
};

#endif
