#include "triangle_mesh_geometry.hpp"
#include "objects/triangle_mesh.hpp"

namespace RT_ISICG
{
	TriangleMeshGeometry::TriangleMeshGeometry( const unsigned int p_v0,
												const unsigned int p_v1,
												const unsigned int p_v2,
												MeshTriangle *	   p_refMesh )
		: _v0( p_v0 ), _v1( p_v1 ), _v2( p_v2 ), _refMesh( p_refMesh )
	{
		_faceNormal = glm::normalize( glm::cross( _refMesh->_vertices[ p_v1 ] - _refMesh->_vertices[ p_v0 ],
												  _refMesh->_vertices[ p_v2 ] - _refMesh->_vertices[ p_v0 ] ) );
	}

	bool TriangleMeshGeometry::intersect( const Ray & p_ray, float & p_t, Vec2f & uv ) const
	{
		const Vec3f & o	 = p_ray.getOrigin();
		const Vec3f & d	 = p_ray.getDirection();
		const Vec3f & v0 = _refMesh->_vertices[ _v0 ];
		const Vec3f & v1 = _refMesh->_vertices[ _v1 ];
		const Vec3f & v2 = _refMesh->_vertices[ _v2 ];
		
		constexpr float epsilon = std::numeric_limits<float>::epsilon();

		Vec3f  edge1		   = v1 - v0;
		Vec3f edge2		   = v2 - v0;
		Vec3f  ray_cross_e2	   = cross( d, edge2 );
		float det		   = dot( edge1, ray_cross_e2 );

		if ( det > -epsilon && det < epsilon ) return false; // This ray is parallel to this triangle.

		float inv_det = 1.0 / det;
		Vec3f s		  = o - v0;
		float u		  = inv_det * dot( s, ray_cross_e2 );

		if ( u < 0 || u > 1 ) return false;

		Vec3f s_cross_e1 = cross( s, edge1 );
		float v			 = inv_det * glm::dot( d, s_cross_e1 );

		if ( v < 0 || u + v > 1 ) return false;

		// At this stage we can compute t to find out where the intersection point is on the line.
		float t = inv_det * dot( edge2, s_cross_e1 );


		if ( t > epsilon ) // ray intersection
		{
			p_t = t;
			uv	= Vec2f( u, v );
			return true;
		}
		else // This means that there is a line intersection but not a ray intersection.
			p_t = -1.f;
			return false;
	}

	const Vec3f TriangleMeshGeometry::getFaceNormal( Vec2f & uv ) const
	{
			const Vec3f & n0 = _refMesh->_normals[ _v0 ];
			const Vec3f & n1 = _refMesh->_normals[ _v1 ];
			const Vec3f & n2 = _refMesh->_normals[ _v2 ];

			return ( 1.f - uv.x - uv.y ) * n0 + uv.x * n1 + uv.y * n2;
	}

} // namespace RT_ISICG
