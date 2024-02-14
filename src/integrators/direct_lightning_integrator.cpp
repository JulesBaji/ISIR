#include "direct_lightning_integrator.hpp"

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
			LightList light_list = p_scene.getLights();		
			return _directLighting( p_scene, p_tMin, p_tMax, hitRecord, light_list );
		}
		else { return _backgroundColor; }
	}

	Vec3f DirectLightingIntegrator::_directLighting( const Scene &	   p_scene,
													 const float						  p_tMin,
													 const float						  p_tMax,
													 const HitRecord & hitRecord,
													 const LightList & light_list ) const
	{
			Vec3f li = VEC3F_ZERO;
			for ( BaseLight * bl : light_list )
			{
				LightSample ls		 = bl->sample( hitRecord._point );
				float		cosTheta = glm::max( glm::dot( hitRecord._normal, ls._direction ), 0.f );
				Ray			shadowRay = Ray( hitRecord._point, ls._direction );
				shadowRay.offset( hitRecord._normal );
				if ( !p_scene.intersectAny( shadowRay, p_tMin, ls._distance ) )
					li += hitRecord._object->getMaterial()->getFlatColor() * ls._radiance * cosTheta;

				if (bl->getIsSurface() == true)
				{
					for (int i = 0; i < _nbLightSamples; i++)
					{
						LightSample ls		  = bl->sample( hitRecord._point );
						float		cosTheta  = glm::max( glm::dot( hitRecord._normal, ls._direction ), 0.f );
						Ray			shadowRay = Ray( hitRecord._point, ls._direction );
						shadowRay.offset( hitRecord._normal );
						if ( !p_scene.intersectAny( shadowRay, p_tMin, ls._distance ) )
							li += hitRecord._object->getMaterial()->getFlatColor() * ls._radiance * cosTheta;
					}	
					li /= (float)_nbLightSamples;
				}
			}
			return li;
	}

} // namespace RT_ISICG
