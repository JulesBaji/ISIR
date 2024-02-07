#pragma once
#ifndef __RT_ISICG_DIRECT_LIGHTNING_INTEGRATOR__
#define __RT_ISICG_DIRECT_LIGHTNING_INTEGRATOR__

#include "base_integrator.hpp"

namespace RT_ISICG
{
	class DirectLightingIntegrator : public BaseIntegrator
	{
	  public:
		DirectLightingIntegrator() : BaseIntegrator() {}
		virtual ~DirectLightingIntegrator() = default;

		const IntegratorType getType() const override { return IntegratorType::RAY_CAST; }

		// Return incoming luminance.
		Vec3f Li( const Scene & p_scene, const Ray & p_ray, const float p_tMin, const float p_tMax ) const override;

		private:
			Vec3f _directLighting() 
			{ 
				//Li += mtl->getColor() * lightSample._radiance * cosTheta;
			}
	};

} // namespace RT_ISICG

#endif // __RT_ISICG_DIRECT_LIGHTNING_INTEGRATOR__
