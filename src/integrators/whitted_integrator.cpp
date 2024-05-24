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
			return _whittedLighting( p_scene, p_ray, p_tMin, p_tMax, hitRecord, 0, false );
		}
		else { return _backgroundColor; }
	}

	Vec3f WhittedIntegrator::_whittedLighting( const Scene &	 p_scene,
													 const Ray &	   p_ray,
													 const float	   p_tMin,
													 const float	   p_tMax,
													 HitRecord hitRecord,
													 const int		 p_nbBounces, 
													 const bool p_inMaterial) const
	{
		if ( p_nbBounces >= _nbBounces ) return hitRecord._object->getMaterial()->getFlatColor();

		if ( p_scene.intersect( p_ray, p_tMin, p_tMax, hitRecord ) )
		{ 
			if ( hitRecord._object->getMaterial()->isMirror() )
			{
				Ray reflection_ray = Ray( hitRecord._point, glm::reflect( p_ray.getDirection(), hitRecord._normal ) );
				reflection_ray.offset( hitRecord._normal );
				
				return _whittedLighting( p_scene, reflection_ray, p_tMin, p_tMax, hitRecord, p_nbBounces + 1, p_inMaterial);
			}
			else if ( hitRecord._object->getMaterial()->isTransparent() )
			{
				Ray reflection_ray = Ray( hitRecord._point, glm::reflect( p_ray.getDirection(), hitRecord._normal ) );
				reflection_ray.offset( hitRecord._normal );
				Vec3f reflectionColor = _whittedLighting(
					p_scene, reflection_ray, p_tMin, p_tMax, hitRecord,
					p_nbBounces + 1, p_inMaterial );

				const float n1 = p_inMaterial ? hitRecord._object->getMaterial()->getIOR() : 1.f;
				const float n2 = !p_inMaterial ? hitRecord._object->getMaterial()->getIOR() : 1.f;

				float cos_theta_i = glm::dot( -p_ray.getDirection(), hitRecord._normal );

				float cos_theta_t = n1 / n2
					  * sqrtf( std::max( 0.f, 1.f - ( 1.f - cos_theta_i * cos_theta_i ) * ( n1 / n2 ) * ( n1 / n2 ) ) );
				
				float sqrt_rs = ( n1 * cos_theta_i - n2 * cos_theta_t ) / ( n1 * cos_theta_i + n2 * cos_theta_t );			
				float sqrt_rp = ( n1 * cos_theta_t - n2 * cos_theta_i ) / ( n1 * cos_theta_t + n2 * cos_theta_i );

				const float kr = ( sqrt_rs * sqrt_rs + sqrt_rp * sqrt_rp ) / 2.f;
				if ( kr == 1.f ) return reflectionColor;

				const Vec3f refractionDir  = glm::refract( p_ray.getDirection(), hitRecord._normal, n1 / n2 );
				Ray refraction_ray = Ray( hitRecord._point, refractionDir );
				refraction_ray.offset( -hitRecord._normal );
				Vec3f refractionColor = _whittedLighting(
					p_scene, refraction_ray, p_tMin, p_tMax, hitRecord, p_nbBounces + 1, !p_inMaterial );

				return reflectionColor * kr + refractionColor * ( 1.f - kr );
			}
			else
				return _directLighting( p_scene, p_ray, p_tMin, p_tMax, hitRecord );
		}
		return _backgroundColor;
	}
} // namespace RT_ISICG

