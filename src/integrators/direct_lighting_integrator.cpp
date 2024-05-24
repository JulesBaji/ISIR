#include "direct_lighting_integrator.hpp"

namespace RT_ISICG
{
	Vec3f DirectLightingIntegrator::Li( const Scene & p_scene,
								 const Ray &   p_ray,
								 const float   p_tMin,
								 const float   p_tMax ) const
	{
		HitRecord hitRecord;
		if ( p_scene.intersect( p_ray, p_tMin, p_tMax, hitRecord ) )
		{			
			return _directLighting( p_scene, p_ray, p_tMin, p_tMax, hitRecord );
		}
		else { return _backgroundColor; }
	}

	Vec3f DirectLightingIntegrator::_directLighting( const Scene &	   p_scene,
													 const Ray &	   p_ray,
													 const float						  p_tMin,
													 const float						  p_tMax,
													 const HitRecord & hitRecord ) const
	{
			LightList light_list = p_scene.getLights();	
			Vec3f li = VEC3F_ZERO;
			for ( BaseLight * bl : light_list )
			{
				Vec3f liTemp = VEC3F_ZERO;
				if (bl->getIsSurface() == true)
				{
					for (int i = 0; i < _nbLightSamples; i++)
					{
						LightSample ls		  = bl->sample( hitRecord._point );
						float		cosTheta  = glm::max( glm::dot( hitRecord._normal, ls._direction ), 0.f );
						Ray			shadowRay = Ray( hitRecord._point, ls._direction );
						shadowRay.offset( hitRecord._normal );
						if ( !p_scene.intersectAny( shadowRay, p_tMin, ls._distance ) )
							liTemp += hitRecord._object->getMaterial()->shade(p_ray, hitRecord, ls) * ls._radiance * cosTheta;
					}	
					liTemp /= (float)_nbLightSamples;
					li += liTemp;
				}
				else
				{
					LightSample ls		  = bl->sample( hitRecord._point );
					float		cosTheta  = glm::max( glm::dot( hitRecord._normal, ls._direction ), 0.f );
					Ray			shadowRay = Ray( hitRecord._point, ls._direction );
					shadowRay.offset( hitRecord._normal );
					if ( !p_scene.intersectAny( shadowRay, p_tMin, ls._distance ) )
						li += hitRecord._object->getMaterial()->shade( p_ray, hitRecord, ls ) * ls._radiance * cosTheta;
				}
			}
			return li;
	}

} // namespace RT_ISICG
