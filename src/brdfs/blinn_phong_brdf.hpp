#ifndef __RT_ISICG_BRDF_BLINN_PHONG__
#define __RT_ISICG_BRDF_BLINN_PHONG__

#include "defines.hpp"

namespace RT_ISICG
{
	class BlinnPhongBRDF
	{
	  public:
		BlinnPhongBRDF( const Vec3f & p_ks ) : _ks( p_ks ) {};

		// * INV_PIf : could be done in the constructor...
		inline Vec3f evaluate( const Ray &		   p_ray,
							   const HitRecord &   p_hitRecord,
							   const LightSample & p_lightSample ) const
		{
			Vec3f wi = p_lightSample._direction;
			Vec3f wo = -p_ray.getDirection();
			Vec3f n	 = p_hitRecord._normal;
			Vec3f wr = glm::reflect( -wi, n );

			float cos_theta_i = glm::dot( wi, n );
			if ( cos_theta_i <= 0 ) return BLACK;

			Vec3f h = glm::normalize(wi + wo );

			float cos_alpha = glm::max( 0.f, glm::dot( n, h ) );

			return _ks / cos_theta_i * glm::pow( cos_alpha, s );
		}

		inline const Vec3f & getKs() const { return _ks; }

	  private:
		Vec3f		_ks = WHITE;
		const float s	= 8.f;
	};
} // namespace RT_ISICG

#endif // __RT_ISICG_BRDF_BLINN_PHONG__
