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

	bool TriangleMeshGeometry::intersect( const Ray & p_ray, float & p_t ) const
	{
		const Vec3f & o	 = p_ray.getOrigin();
		const Vec3f & d	 = p_ray.getDirection();
		const Vec3f & v0 = _refMesh->_vertices[ _v0 ];
		const Vec3f & v1 = _refMesh->_vertices[ _v1 ];
		const Vec3f & v2 = _refMesh->_vertices[ _v2 ];

		float epsilon = 0.000001;
		Vec3f edge1 = v1 - v0, edge2 = v2 - v0;
		Vec3f pvec = glm::cross( d, edge2 );
		float det  = glm::dot( edge1, pvec );
		// J'ai fait que le code avec culling
		if ( det < epsilon ) return false;
		Vec3f tvec = o - v0;
		float u	   = glm::dot( tvec, pvec );
		if ( u < 0 || u > det ) return false;
		Vec3f qvec = glm::cross( tvec, edge1 );
		float v	   = glm::dot( d, qvec );
		if ( v < 0 || u + v > det ) return false;
		float t = glm::dot( edge2, qvec );
		float inv_det = 1.f / det;
		t *= inv_det;
		u *= inv_det;
		v *= inv_det;
		//fin avec culling
		return true;
	}

} // namespace RT_ISICG
