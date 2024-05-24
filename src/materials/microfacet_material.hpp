#ifndef __RT_ISICG_MICROFACET_MATERIAL__
#define __RT_ISICG_MICROFACET_MATERIAL__

#include "base_material.hpp"
#include "brdfs/oren_nayar_brdf.hpp"
#include "brdfs/cook_torrance_brdf.hpp"

namespace RT_ISICG
{
	class MicrofacetMaterial : public BaseMaterial
	{
	  public:
		MicrofacetMaterial( const std::string & p_name, const Vec3f & p_diffuse, const Vec3f & p_specular, const float p_sigma, const float p_metalness )
			: BaseMaterial( p_name ), _onbrdf( p_diffuse, p_sigma ), _ckbrdf( p_specular, p_sigma ), _metalness(p_metalness)
		{
		}

		virtual ~MicrofacetMaterial() = default;

		Vec3f shade( const Ray &		 p_ray,
					 const HitRecord &	 p_hitRecord,
					 const LightSample & p_lightSample ) const override
		{
			return ( 1 - _metalness ) * _onbrdf.evaluate( p_ray, p_hitRecord, p_lightSample )
				   + _ckbrdf.evaluate( p_ray, p_hitRecord, p_lightSample ) * _metalness;
		}

		inline const Vec3f & getFlatColor() const override { return _ckbrdf.getKs(); }

	  protected:
		CookTorranceBRDF _ckbrdf;
		OrenNayarBRDF	 _onbrdf;
		const float		 _metalness;
	};

} // namespace RT_ISICG

#endif // __RT_ISICG_MICROFACET_MATERIAL__
