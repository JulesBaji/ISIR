#include "whitted_integrator.hpp"

namespace RT_ISICG
{
	Vec3f WhittedIntegrator::Li( const Scene & p_scene,
										const Ray &	  p_ray,
										const float	  p_tMin,
										const float	  p_tMax ) const
	{
		HitRecord hitRecord;
		if ( p_scene.intersect( p_ray, p_tMin, p_tMax, hitRecord ) )
		{
			return _whittedLighting( p_scene, p_ray, p_tMin, p_tMax, hitRecord, 0 );
		}
		else { return _backgroundColor; }
	}

	Vec3f WhittedIntegrator::_whittedLighting( const Scene &	 p_scene,
													 const Ray &	   p_ray,
													 const float	   p_tMin,
													 const float	   p_tMax,
													 HitRecord & hitRecord,
													 const int &		 p_nbBounces ) const
	{
		if ( p_nbBounces >= _nbBounces ) return hitRecord._object->getMaterial()->getFlatColor();

		if ( p_scene.intersect( p_ray, p_tMin, p_tMax, hitRecord ) )
		{ 
			if ( hitRecord._object->getMaterial()->isMirror() )
			{
				Ray reflection_ray = Ray( hitRecord._point, glm::reflect( p_ray.getDirection(), hitRecord._normal ) );
				reflection_ray.offset( hitRecord._normal );
				
				return _whittedLighting( p_scene, reflection_ray, p_tMin, p_tMax, hitRecord, p_nbBounces + 1 );
			}
			else
				return _directLighting( p_scene, p_ray, p_tMin, p_tMax, hitRecord );
		}
		return _backgroundColor;
	}

} // namespace RT_ISICG