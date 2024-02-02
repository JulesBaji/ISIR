#include "perspective_camera.hpp"
#include <glm/gtx/string_cast.hpp>

namespace RT_ISICG
{
	PerspectiveCamera::PerspectiveCamera( const float p_aspectRatio ) : _aspectRatio( p_aspectRatio )
	{
		_updateViewport();
	}

	PerspectiveCamera::PerspectiveCamera( const Vec3f & p_position,
										  const Vec3f & p_lookAt,
										  const Vec3f & p_up,
										  const float	p_fovy,
										  const float	p_aspectRatio )
		: BaseCamera( p_position ), _fovy( p_fovy ), _aspectRatio( p_aspectRatio )
	{
		_w = -glm::normalize( p_lookAt - p_position );
		_u = glm::normalize(glm::cross(p_up, _w ));
		_v = glm::normalize(glm::cross(_w, _u ));

		_updateViewport();
	}

	void PerspectiveCamera::_updateViewport()
	{
		float h				   = 2.f * glm::tan( glm::radians(_fovy) / 2 ) * _focalDistance; // viewPortWidth
		float w				   = h * _aspectRatio;							   // viewPortHeight
		_viewportV = _v * h ;
		_viewportU = _u * w;
		_viewportTopLeftCorner = _position - _w * _focalDistance + _viewportV / 2.f - _viewportU / 2.f;
	}

} // namespace RT_ISICG
