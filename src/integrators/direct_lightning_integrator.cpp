#include "direct_lightning_integrator.hpp"
#include "lights/point_light.hpp"

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
			float cosTheta = glm::max( glm::dot( hitRecord._normal, -p_ray.getDirection() ), 0.f );
			LightList light_list;
			PointLight p = PointLight();
			/*LightSample lightSample = LightSample(p_ray.getDirection(),  )
			Li += hitRecord._object->getMaterial()->getFlatColor() * lightSample._radiance * cosTheta;*/
			return 
		}
		else { return _backgroundColor; }
	}

	Vec3f DirectLightingIntegrator::_directLighting()
	{

	}

} // namespace RT_ISICG
