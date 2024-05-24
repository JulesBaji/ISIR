#ifndef __RT_ISICG_OREN_NAYAR_MATERIAL__
#define __RT_ISICG_OREN_NAYAR_MATERIAL__

#include "base_material.hpp"
#include "brdfs/oren_nayar_brdf.hpp"

namespace RT_ISICG
{
	class MatteMaterial : public BaseMaterial
	{	
	  public:
		MatteMaterial( const std::string & p_name, const Vec3f & p_diffuse, const float p_sigma )
			: BaseMaterial( p_name ), _brdf( p_diffuse, p_sigma )
		{
		}

		virtual ~MatteMaterial() = default;

		Vec3f shade( const Ray &		 p_ray,
					 const HitRecord &	 p_hitRecord,
					 const LightSample & p_lightSample ) const override
		{
			return _brdf.evaluate( p_ray, p_hitRecord, p_lightSample );
		}

		inline const Vec3f & getFlatColor() const override { return _brdf.getKd(); }

	  protected:
		OrenNayarBRDF _brdf;
	};

} // namespace RT_ISICG

#endif // __RT_ISICG_OREN_NAYAR_MATERIAL__
