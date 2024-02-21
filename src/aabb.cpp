#include "aabb.hpp"

namespace RT_ISICG
{
	bool AABB::intersect( const Ray & p_ray, const float p_tMin, const float p_tMax ) const
	{
		double tmin = p_tMin, tmax = p_tMax;

		if ( p_ray.getInvDirection().x != 0 )
		{
			double tx1 = ( _min.x - p_ray.getOrigin().x ) * p_ray.getInvDirection().x;
			double tx2 = ( _max.x - p_ray.getOrigin().x ) * p_ray.getInvDirection().x;

			tmin = glm::min( tx1, tx2 );
			tmax = glm::max( tx1, tx2 );
		}
					
		if ( p_ray.getInvDirection().y != 0 )
		{
			double ty1 = ( _min.y - p_ray.getOrigin().y ) * p_ray.getInvDirection().y;
			double ty2 = ( _max.y - p_ray.getOrigin().y ) * p_ray.getInvDirection().y;

			tmin	   = glm::max( tmin, glm::min( ty1, ty2 ) );
			tmax	   = glm::min( tmax, glm::max( ty1, ty2 ) );
		}
			
		if ( p_ray.getInvDirection().z != 0 )
		{
			double tz1 = ( _min.z - p_ray.getOrigin().z ) * p_ray.getInvDirection().z;
			double tz2 = ( _max.z - p_ray.getOrigin().z ) * p_ray.getInvDirection().z;

			tmin = glm::max( tmin, glm::min( tz1, tz2 ) );
			tmax = glm::min( tmax, glm::max( tz1, tz2 ) );
		}
			
		return tmax >= tmin;
	}
} // namespace _max_ISICG
