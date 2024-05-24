#ifndef __RT_ISICG_BRDF_OREN_NAYAR__
#define __RT_ISICG_BRDF_OREN_NAYAR__

#include "defines.hpp"

namespace RT_ISICG
{
	class OrenNayarBRDF
	{
	  public:
		OrenNayarBRDF( const Vec3f & p_kd, const float p_sigma ) : _kd( p_kd ), _sigma( p_sigma ) {}

		// * INV_PIf : could be done in the constructor...
		inline Vec3f evaluate( const Ray &									p_ray,
							   const HitRecord &							p_hitRecord,
							   const LightSample &							p_lightSample ) const 
		{ 
			const float A = 1.f - 0.5f * (_sigma * _sigma) / ( _sigma * _sigma + 0.33f );
			const float B = 0.45f * ( _sigma * _sigma ) / ( _sigma * _sigma + 0.09f );

			Vec3f wi = p_lightSample._direction;
			Vec3f wo = -p_ray.getDirection();

			float theta_i = glm::acos( glm::dot( p_hitRecord._normal, wi ) );
			float phi_i	  = glm::atan( wi.y, wi.x );

			float theta_o = glm::acos( glm::dot( p_hitRecord._normal, wo ) );
			float phi_o	  = glm::atan( wo.y, wo.x );

			float alpha = std::max( theta_i, theta_o );

			float beta	= std::min( theta_i, theta_o );

			return _kd * INV_PIf
				   * ( A + B * std::max( 0.f, std::cos( phi_i - phi_o ) ) * std::sin( alpha ) * std::tan( beta ) );
		}

		inline const Vec3f & getKd() const { return _kd; }

	  private:
		Vec3f _kd = WHITE;
		const float _sigma;
	};
} // namespace RT_ISICG

#endif // __RT_ISICG_BRDF_OREN_NAYAR__
