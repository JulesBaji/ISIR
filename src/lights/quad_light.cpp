#include "quad_light.hpp"
#include "utils/random.hpp"

namespace RT_ISICG
{
	LightSample QuadLight::sample( const Vec3f & p_point ) const
	{
		Vec3f positionAl	= _position + randomFloat() * _u + randomFloat() * _v;
		Vec3f direction		= glm::normalize( positionAl - p_point );
		float distance		= glm::distance( positionAl, p_point );
		float pdf			= 1.f / (_area) * (distance * distance) / (std::cos( glm::dot( _n, direction )) );
		Vec3f radiance		= _color * _power / ( pdf );

		return LightSample( direction, distance, radiance, pdf );
	}
} // namespace RT_ISICG