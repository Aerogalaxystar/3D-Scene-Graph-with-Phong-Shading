#ifndef PYRAMID_H
#define PYRAMID_H

#include "GenericObject.h"

class Pyramid : public GenericObject {
public:
    Pyramid(glm::vec3 pos, glm::vec3 col)
        : GenericObject(pos, col) {
    }

    void Render(const glm::mat4& projection, const glm::mat4& view) override;
};

#endif
