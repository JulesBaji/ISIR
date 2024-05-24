#ifndef __RT_ISICG_WHITTED_INTEGRATOR__
#define __RT_ISICG_WHITTED_INTEGRATOR__

#include "direct_lighting_integrator.hpp"

namespace RT_ISICG
{
	class WhittedIntegrator : public DirectLightingIntegrator
	{
	  public:
		WhittedIntegrator() : DirectLightingIntegrator() {}
		virtual ~WhittedIntegrator() = default;

		const IntegratorType getType() const override { return IntegratorType::WHITTED_LIGHTING; }

		// Return incoming luminance.
		Vec3f Li( const Scene & p_scene, const Ray & p_ray, const float p_tMin, const float p_tMax ) const override;

	  private:
		Vec3f _whittedLighting( const Scene &	 p_scene,
							   const Ray &		 p_ray,
							   const float		 p_tMin,
							   const float		 p_tMax,
							   HitRecord & hitRecord,
							   const int &		  p_nbBounces ) const;

		const int _nbLightSamples = 1;
		const int _nbBounces	  = 5;
	};

} // namespace RT_ISICG

#endif // __RT_ISICG_WHITTED_INTEGRATOR__
