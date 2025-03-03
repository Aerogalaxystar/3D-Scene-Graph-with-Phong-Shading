#ifndef SCENEGRAPH_H
#define SCENEGRAPH_H

#include <vector>
#include "GenericObject.h"


class SceneGraph {
public:
    std::vector<GenericObject*> objects;  // Container for all root objects

    // Constructor and Destructor
    SceneGraph() = default;
    ~SceneGraph() {
        // Clean up allocated memory for objects
        for (auto* obj : objects) {
            delete obj;
        }
    }

    // Add an object to the root list
    void AddObject(GenericObject* obj) {
        objects.push_back(obj);
    }

    // Render all objects (and their children)
    void Render(const glm::mat4& projection, const glm::mat4& view) {

        for (auto* obj : objects) {
            obj->RenderWithChildren(projection, view);  // Render recursively with children
        }
    }
};

#endif
