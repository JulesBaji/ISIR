#ifndef __RT_ISICG_PATH_TRACING_INTEGRATOR__
#define __RT_ISICG_PATH_TRACING_INTEGRATOR__

#include "whitted_integrator.hpp"

namespace RT_ISICG
{
	class PathTracingIntegrator : public WhittedIntegrator
	{
	  public:
		PathTracingIntegrator() : WhittedIntegrator() {}
		virtual ~PathTracingIntegrator() = default;

		const IntegratorType getType() const override { return IntegratorType::PATH_TRACING; }

		// Return incoming luminance.
		Vec3f Li( const Scene & p_scene, const Ray & p_ray, const float p_tMin, const float p_tMax ) const override;

	  private:
		Vec3f _pathTracing( const Scene & p_scene,
								const Ray &	  p_ray,
								const float	  p_tMin,
								const float	  p_tMax,
								HitRecord	  hitRecord,
								const int p_depth,
								const bool p_inMaterial ) const;

		Vec3f _indirectLighting( const Scene &	 p_scene,
						   const Ray &		 p_ray,
						   const float		 p_tMin,
						   const float		 p_tMax,
						   HitRecord hitRecord,
						   const int		   p_nbBounces,
						   const bool	   p_inMaterial ) const;

		Vec3f sampleHemisphere( const Vec3f & normal ) const;

		Vec3f _whittedLighting( const Scene & p_scene,
						  const Ray &	p_ray,
						  const float	p_tMin,
						  const float	p_tMax,
						  HitRecord		hitRecord,
						  const int		p_nbBounces,
						  const bool	p_inMaterial) const;

		const int _nbLightSamples = 1;
		const int _nbBounces	  = 4;
		const int _nbRays		  = 16;
	};

} // namespace RT_ISICG

#endif // __RT_ISICG_PATH_TRACING_INTEGRATOR__
