#include "implicit_surface.hpp"

namespace RT_ISICG
{
	bool ImplicitSurface::intersect( const Ray & p_ray,
									 const float p_tMin,
									 const float p_tMax,
									 HitRecord & p_hitRecord ) const
	{
		float step	   = p_tMin;
		Vec3f position = p_ray.getOrigin() + step * p_ray.getDirection();
		float		distance   = _sdf( position );

		while ( step <= p_tMax )
		{
			position = p_ray.getOrigin() + step * p_ray.getDirection();
			distance = _sdf( position );

			if (distance <= _minDistance)
			{
				// Intersection found, fill p_hitRecord.
				p_hitRecord._point	= position;
				p_hitRecord._normal = _evaluateNormal( p_hitRecord._point );
				p_hitRecord.faceNormal( p_ray.getDirection() );
				p_hitRecord._distance = distance;
				p_hitRecord._object	  = this;

				return true;
			}
			step += distance;
		}
		return false;
	}

	bool ImplicitSurface::intersectAny( const Ray & p_ray, const float p_tMin, const float p_tMax ) const
	{
		float step	   = p_tMin;
		Vec3f position = p_ray.getOrigin() + step * p_ray.getDirection();
		float distance = _sdf( position );

		while ( step <= p_tMax )
		{
			position = p_ray.getOrigin() + step * p_ray.getDirection();
			distance = _sdf( position );

			if ( distance <= _minDistance )
			{
				return true;
			}
			step += distance;
		}
		return false;
	}
} // namespace RT_ISICG
