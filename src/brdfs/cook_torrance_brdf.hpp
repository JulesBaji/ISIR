#ifndef __RT_ISICG_BRDF_COOK_TORRANCE__
#define __RT_ISICG_BRDF_COOK_TORRANCE__

#include "defines.hpp"

namespace RT_ISICG
{
	class CookTorranceBRDF
	{
	  public:
		CookTorranceBRDF( const Vec3f & p_ks, const float p_sigma ) : _ks( p_ks ), _sigma(p_sigma) {};

		// * INV_PIf : could be done in the constructor...
		inline Vec3f evaluate( const Ray &		   p_ray,
							   const HitRecord &   p_hitRecord,
							   const LightSample & p_lightSample ) const
		{
			Vec3f wi	= p_lightSample._direction;
			Vec3f wo	= -p_ray.getDirection();
			Vec3f n		= p_hitRecord._normal;
			Vec3f h = glm::normalize( wi + wo );
			float k			= ( ( _sigma + 1 ) * ( _sigma + 1 ) ) / 8;
			float alpha = _sigma * _sigma;
			float alphaSqrd = alpha * alpha;

			float D = alphaSqrd / ( PIf * ( glm::dot( n, h ) * glm::dot( n, h ) * ( alphaSqrd - 1 ) + 1.f )
						  * ( glm::dot( n, h ) * glm::dot( n, h ) * ( alphaSqrd - 1 ) + 1.f ) );
			float G = getG1( glm::dot( n, wo ), k ) * getG1( glm::dot( n, wi ), k );
			Vec3f F = _ks + ( 1.f - _ks ) * glm::pow( ( 1.f - glm::dot( h, wo ) ), 5.f );

			return ( D * F * G ) / ( 4.f * glm::dot( wo, n ) * glm::dot( wi, n ) );
		}

		inline const Vec3f & getKs() const { return _ks; }

	  private:
		inline const float & getG1( float x, float k ) const { return x / (x*(1-k) + k); }

		Vec3f		_ks = WHITE;
		const float _sigma;
	};
} // namespace RT_ISICG

#endif // __RT_ISICG_BRDF_COOK_TORRANCE__