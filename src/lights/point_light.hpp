#pragma once
#ifndef __RT_ISICG_POINT_LIGHT__
#define __RT_ISICG_POINT_LIGHT__

#include "base_light.hpp"

namespace RT_ISICG
{
	class PointLight : public BaseLight
	{
	  public:

		PointLight() = default;

		virtual ~PointLight() = default;

		inline const Vec3f & getPosition() const { return _position; }

		LightSample sample( const Vec3f & p_point );

	  protected:
		// TODO: give a name, like objects and materials
		Vec3f _position = VEC3F_ZERO;
	};
} // namespace RT_ISICG

#endif // __RT_ISICG_POINT_LIGHT__
