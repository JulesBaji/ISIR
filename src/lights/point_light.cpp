#include "point_light.hpp"

namespace RT_ISICG
{
	LightSample PointLight::sample( const Vec3f & p_point )
	{ 
		Vec3f direction = _position - p_point;
		float distance = glm::distance( _position, p_point );
		Vec3f radiance	= _color * _power / (distance*distance);
		float pdf		= 1;
		 		
		return LightSample( direction, distance, radiance, pdf );
		
	}
} // namespace RT_ISICG