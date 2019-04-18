#include "Light.h"

//Light constructor : no parameter is necessary (default : white light at (0,0,0))
Light::Light(glm::vec3 position, float intensity, glm::vec4 color) :
	Positionable(position), m_intensity(intensity), m_color(color) {
}
