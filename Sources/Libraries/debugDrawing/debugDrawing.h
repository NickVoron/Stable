#pragma once

#include <D3DX9.h>

#include "stuff/stringUtils.h"

#include "gapi/library.include.h"
#include "math/library.include.h"
#include "newmath/library.include.h"
#include "intersections/library.include.h"
#include "geometry/library.include.h"
#include "viewports/library.include.h"
#include "image/library.include.h"

#include "types.h"

namespace DebugDrawing
{
	void init(const char* fxFileName, const char* fontName, gapi::Id api);
	void release();
	void flush();
	void enable(bool enabled);
	bool& enabled();

	void immediateMode(bool enabled);
	void shaderMode(ShaderMode mode);

	//
	void ignoreViewTransform(bool value);
	
	//
	Vector4 color();
	void color(const Vector3& color);
	void color(const Vector4& color);
	void color(const Geometry::DIFFUSE& color);
	void color_rgb(float r, float g, float b);
	void color_hsv(float h, float s, float v);
	void color_rgba(float r, float g, float b, float a);
	void color_hsva(float h, float s, float v, float a);

	template<image::Format colorFormat>
	void color(const image::Color<colorFormat>& color)
	{
		image::Color<image::RGBA32F> clr = color;
		color_rgb(clr.r(), clr.g(), clr.b());
	}

	//
	void text(const Vector3& worldCoord, const std::string& text);
	void text(const Vector3& worldCoord, const nm::index2& screenShift, const std::string& text);
	void text(const nm::index2& screenCoord, const std::string& text);
	
	template<class... T> void text(const Vector3& worldCoord, const T&... obj)								  { text(worldCoord, str::stringize(obj...).str() ); }
	template<class... T> void text(const Vector3& worldCoord, const nm::index2& screenShift, const T&... obj) { text(worldCoord, screenShift, str::stringize(obj...).str() ); }
	template<class... T> void text(const nm::index2& screenCoord, const T&... obj)							  { text(screenCoord, str::stringize(obj...).str() ); }

	//
	void line(const Vector3& start, const Vector3& end);
	void line2(const nm::index2& start, const nm::index2& end);
	void lineRect(const unigui::Rect& screenRect);
	void rect(const unigui::Rect& screenRect);

	void spline(const nm::spline& spln, bool renderVertices);

	void lineloop_impl(const nm::Vector3* p0, std::size_t pointsCount);

	template<class... VectorType>
	void lineloop(const VectorType&... p0)
	{
		nm::Vector3 points [] = { nm::Vector3(p0)... };
		lineloop_impl(&points[0], sizeof...(p0));
	}


	template<class VertexComponents> void line(const Geometry::Vertex<VertexComponents>& p0, const Geometry::Vertex<VertexComponents>& p1) { line(Vector3(p0.pos3().x, p0.pos3().y, p0.pos3().z), Vector3(p1.pos3().x, p1.pos3().y, p1.pos3().z)); }

	template<class... VertexComponents>
	void lineloop(const Geometry::Vertex<VertexComponents>&... p0)
	{ 
		nm::Vector3 points[] = { p0.pos3()...}; 
		lineloop_impl(&points[0], sizeof...(p0));
	}
	
	//
	void wireBox(const State& s, const Vector3& extents);
	void wireBox(const State& s, float extents);
	void wireBox(const Vector3& position, const Vector3& extents);
	void wireBox(const Vector3& s, float extents);
	void wireBox(const Vector3& position);
	void wireBox(const intersections::OBB& obb);
	void wireBox(const intersections::AABB& obb);

	void wirePrysm(const Vector3* base, int baseCount, const Vector3& edge);

	template<int vcount>
	void wirePrysm(const intersections::Prysm<vcount>& prysm)
	{
		wirePrysm(prysm.base, prysm.baseVerticesCount, prysm.edge);
	}

	void mesh(const MeshType& mesh, float scale = 1.0f);
	void mesh(const MeshType& mesh, const State& s, float scale = 1.0f);
	void mesh(const MeshType& mesh, const Vector3& position, float scale = 1.0f);

	template<class VertexT, class IndexT>
	void mesh(const Geometry::Mesh<VertexT, IndexT>& usermesh, const State& s, float scale = 1.0f)
	{
		MeshType internalMesh;
		internalMesh.maxAssign(usermesh);
		mesh(internalMesh, s, scale);
	}

	template<class VertexT, class IndexT>	void mesh(const Geometry::Mesh<VertexT, IndexT>& usermesh, const Vector3& position, float scale = 1.0f)	{ mesh(usermesh, State::Simple(position), scale); }
	template<class VertexT, class IndexT>	void mesh(const Geometry::Mesh<VertexT, IndexT>& usermesh, float scale = 1.0f)							{ mesh(usermesh, State::Identity(), scale); }

	void lines(const MeshType& usermesh, float scale = 1.0f);
	void lines(const MeshType& usermesh, const State& s, float scale = 1.0f);
	void lines(const MeshType& usermesh, const Vector3& position, float scale = 1.0f);

	template<class VertexT, class IndexT>
	void lines(const Geometry::Mesh<VertexT, IndexT>& usermesh, const State& s, float scale = 1.0f)
	{
		MeshType internalMesh;
		internalMesh.maxAssign(usermesh);
		lines(internalMesh, s, scale);
	}			 

	template<class VertexT, class IndexT>	void lines(const Geometry::Mesh<VertexT, IndexT>& usermesh, const Vector3& position, float scale = 1.0f)	{ lines(usermesh, State::Simple(position), scale); }
	template<class VertexT, class IndexT>	void lines(const Geometry::Mesh<VertexT, IndexT>& usermesh, float scale = 1.0f)								{ lines(usermesh, State::Identity(), scale); }

	void triangle(const intersections::Triangle& triangle);
	void triangle(const nm::Vector3* vertices);
	void quad(const intersections::Quad& quad);
	void ray(const intersections::Ray& ray);
	

	void box(const State& s, const Vector3& extents);
	void box(const Vector3& position, const Vector3& radius);
	void box(const Vector3& position);

	void sphere(const intersections::Sphere& s);
	void sphere(const State& s, float radius);
	void sphere(const Vector3& position, float radius);
	void sphere(const Vector3& position);
	void sphere(const nm::index3& position);
	void sphere(const nm::index3& position, float radius);


	void cylinder(const State& s, float radius, float height);
	void cylinder(const Vector3& position, float radius, float height);
	void cylinder(const Vector3& position);

	void cone(const State& s, float radius, float height);
	void cone(const Vector3& position, float radius, float height);
	void cone(const Vector3& position);
	void cone(const Vector3& locus, const Vector3& direction, float radius);
	void cone(const Vector3& locus, float angle, const Vector3& direction);

	void coordinatesGrid(const Base::Interval<int>& bounds, float delimSize);

	void arrow(const Vector3& from, const Vector3& to, float width);

	void locator(const State& s, const Vector3& radius);
	void locator(const State& s);

	void camera(const State& s, float size);
	void camera(const CameraSystem::Params& cam, float size, bool drawFrustum);
	void camera(const CameraSystem::UserCameraBase& cam, float size, bool drawFrustum);
};