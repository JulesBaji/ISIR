#include "plane_geometry.hpp"

namespace RT_ISICG
{
	bool PlaneGeometry::intersect( const Ray & p_ray, float & p_t1) const
	{
		// On suppose les vecteurs normalises
		float den = glm::dot(_normale, p_ray.getDirection());
		// Si le plan et le rayon ne sont pas parallèles
		if ( den != 0 )
		{ // Attention à la comparaison avec 0
			Vec3f  po = _center - p_ray.getOrigin();
			p_t1	 = glm::dot( po, _normale ) / den;
			return p_t1 > 0 ? true : false; // Si intersection devant
		}
		// Sinon, pas d’intersection
		return false;
	}
} // namespace RT_ISICG
