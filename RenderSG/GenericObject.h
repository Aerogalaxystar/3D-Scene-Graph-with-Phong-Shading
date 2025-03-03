#ifndef GenericObjectH
#define GenericObjectH

#include <glm/glm.hpp>
#include <vector>
class GenericObject {

   public:
	   glm::vec3 position;
	   glm::vec3 color;
	   std::vector<GenericObject*>o_objects;
	   GenericObject(glm::vec3 pos, glm::vec3 col) : position(pos), color(col) {}

	   virtual ~GenericObject() {
		   for (auto* child : o_objects)
		   {
			   delete child;
		   }
        }

	   // Add child object to this object
	   void AddChild(GenericObject* child) {
		   o_objects.push_back(child);
	   }
       
	   // Virtual render method to be overridden by child classes
	   virtual void Render(const glm::mat4& projection, const glm::mat4& view) = 0;

	   // Render this object and recursively render its children
	   void RenderWithChildren(const glm::mat4& projection, const glm::mat4& view) {
		   // Render the object itself
		   Render(projection, view);

		   // Then render all children recursively
		   for (auto* child : o_objects) {
			   child->RenderWithChildren(projection, view);
		   }
	   }


};
#endif // !GenericObjectH

