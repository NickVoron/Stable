// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "debugDrawing.h"


#include "imgui/library.include.h"
#include "uniguidraw/library.include.h"
#include "geometry/library.include.h"
#include "common/clearPtrContainer.h"

#include "api.h"


#define CHECK_INITIALIZED if(!inited) return;

namespace DebugDrawing
{

	namespace 
	{
		
		namespace PredefinedColors 
		{
			const Vector3 red	(1.0f, 0.0f, 0.0f);
			const Vector3 green	(0.0f, 1.0f, 0.0f);
			const Vector3 blue	(0.0f, 0.0f, 1.0f);
			const Vector3 white	(1.0f, 1.0f, 1.0f);
			const Vector3 yellow(1.0f, 1.0f, 0.0f);
			const Vector3 gray	(0.5f, 0.5f, 0.5f);
		} 

		struct LinesBuffer 
		{
			Geometry::VertexStream<DebugDrawing::VertexType> lineVS;
			
			void add(const Vector4& color, const Vector3& p0, const Vector3& p1)
			{
				auto bufferSize = lineVS.size();
				
				if(lineVS.capacity() < bufferSize + 2)
				{
					lineVS.reserve(bufferSize * 2);
				}
				
				lineVS.resize(bufferSize + 2);

				auto& v0 = lineVS[bufferSize + 0];
				auto& v1 = lineVS[bufferSize + 1];

				v0.pos3(p0);
				v0.diffuse(color);

				v1.pos3(p1);
				v1.diffuse(color);
			}

			void reset()
			{
				lineVS.resize(0);
			}
		};

		
		LinesBuffer linesBuffer;
		ShaderDataPerCamera shaderDataPerCamera;
		ShaderDataPerObject shaderDataPerObject;
		

		ShaderMode currentShaderMode = INTERNAL_DEFAULT_SHADER;
		
		std::unique_ptr<ToolSwitcher> tool;

		bool immediateModeEnabled = true;
		bool inited = false;

		void drawTransformedLine(const Vector3& a, const Vector3& b, const State& s, const Vector3& radius )
		{
			Vector3 ta = s.GetLocalVector( a.GetPerComponentMul( radius ) );
			Vector3 tb = s.GetLocalVector( b.GetPerComponentMul( radius ) );
			line( ta, tb );
		}

		void uploadCameraParams(Viewports::Viewport& viewport)
		{
			viewport.set();

			Viewports::CalculatedCamera calculatedCamera = viewport.calculateCamera();

			shaderDataPerCamera.view = calculatedCamera.view;
			shaderDataPerCamera.projection = calculatedCamera.projection;
			shaderDataPerCamera.direction = Vector4(calculatedCamera.direction.x, calculatedCamera.direction.y, calculatedCamera.direction.z);

			tool->uploadPerCamera(viewport, shaderDataPerCamera);
		}

		template<bool triangles, class DrawData, class ScaleType>
		DrawableObject* drawBody(const DrawData& mesh, const State& s, const ScaleType& scale, bool updateDataBuffer)
		{
			if (!inited || !enabled()) return 0;

			if(immediateModeEnabled)
			{
				for (auto& vp : Viewports::viewports)
				{					
					uploadCameraParams(vp);
					shaderDataPerObject.setWorld(s, scale);
					DrawableObject* obj = tool->object<triangles>(mesh, shaderDataPerObject, updateDataBuffer);
					obj->draw(vp, currentShaderMode);

					delete obj;
				}

				return nullptr;
			}
			else
			{
				shaderDataPerObject.setWorld(s, scale);
				return tool->object<triangles>(mesh, shaderDataPerObject, updateDataBuffer);
			}			
		}
	}

	DrawableObject* endDrawLinesImpl()
	{
		if(!inited) return 0;
		
		shaderDataPerObject.colorFromVertex[0] = 1;
		DrawableObject* obj = drawBody<false>(linesBuffer.lineVS, State::Identity(), 1.0f, true);
		linesBuffer.reset();
		shaderDataPerObject.colorFromVertex[0] = 0;
	
		return obj;
	}

	void addLineImpl(const Vector4& color, const Vector3& start, const Vector3& end)
	{
		CHECK_INITIALIZED;

		linesBuffer.add(color, start, end);
	}

	void endDrawLines()
	{
		if(immediateModeEnabled)
		{
			endDrawLinesImpl();
		}
	}

	void addLine(const Vector3& start, const Vector3& end)
	{
		Primitives::Line::add(immediateModeEnabled, shaderDataPerObject.color, start, end);
	}

	namespace Primitives
	{
		
		
		
		DrawableObject* draw(const Primitives::Line& p)
		{
			addLineImpl(p.color, p.points[0], p.points[1]);
			return 0;
		}

		DrawableObject* draw(const Primitives::Box& p)
		{
			static MeshType mesh;
			if(mesh.empty())
			{
				Geometry::Generation::generateBox( mesh, Vector3(1.0f, 1.0f, 1.0f) );
			}

			color(p.color);
			return drawBody<true>(mesh, p.state, p.extents * 2.0f, false);
		}

		DrawableObject* draw(const Primitives::Sphere& p)
		{
			static MeshType mesh;
			if(mesh.empty())
			{
				Geometry::Generation::generateSphere(mesh,  1.0f, 3);
			}

			color(p.color);
			return drawBody<true>(mesh, p.state, p.radius, false);
		}

		DrawableObject* draw(const Primitives::Cylinder& p)
		{
			static MeshType mesh;
			if(mesh.empty())
			{
				Geometry::Generation::generateCylinder(mesh, 1.0f, 1.0f, 36, 1);
			}

			color(p.color);
			return drawBody<true>(mesh, p.state, Vector3(p.radius, p.height, p.radius), false);
		}

		DrawableObject* draw(const Primitives::Cone& p)
		{
			static MeshType mesh;
			if(mesh.empty())
			{
				Geometry::Generation::generateCone(mesh, 0.0f, 1.0f, 1.0f, 36, 1);
			}

			color(p.color);
			return drawBody<true>(mesh, p.state, Vector3(p.radius, p.height, p.radius), false);
		}

		DrawableObject* draw(const Primitives::Mesh& p)
		{
			color(p.color);
			drawBody<true>(p.mesh, p.state, p.scale, true);

			return 0;
		}

		DrawableObject* draw(const Primitives::LineMesh& p)
		{
			color(p.color);
			drawBody<false>(p.mesh, p.state, p.scale, true);
			return 0;
		}
	}


	
	void init(const char* fxFileName, const char* fontName, gapi::Id api)
	{
		try
		{
			tool = std::make_unique<ToolSwitcher>();
			tool->init(fxFileName, api);
			inited = true;
		}
		catch (std::exception& e)
		{
			LOG_ERROR_EX(" failed: " << e);
		}
	}

	void release()
	{
		RendererCache::cache().clearData();
	}

	void flush()
	{
		if(!immediateModeEnabled)
		{
 			for (auto& vp : Viewports::viewports)
 			{
 				uploadCameraParams(vp);

				std::list<DrawableObject*> objects;
				Primitives::DrawableLists::get().draw(objects);
				
 				for (DrawableObject* obj : objects)
 				{
 					obj->draw(vp, currentShaderMode);
 				}
				Base::clearPtrContainer(objects);
 			}

			Primitives::DrawableLists::get().reset();
		}
	}

	void enable(bool e)
	{
		enabled() = e;
	}

	bool& enabled()
	{
		static bool value = true;
		return value;
	}

	void immediateMode(bool enabled)
	{
		immediateModeEnabled = enabled;
	}

	void shaderMode(ShaderMode mode)
	{
		currentShaderMode = mode;
	}

	Vector4 color()	{	return shaderDataPerObject.color;	}

	void color(const Vector4& clr)						{ shaderDataPerObject.color = clr;									}
	void color(const Vector3& clr)						{ shaderDataPerObject.color = Vector4(clr.x, clr.y, clr.z, 1.0f);	}
	void color(const Geometry::DIFFUSE& clr)			{ shaderDataPerObject.color = Vector4(clr.r, clr.g, clr.b, clr.a); }
	void color_rgb(float r, float g, float b)			{ color( Vector3(r, g, b) );										}
	void color_rgba(float r, float g, float b, float a)	{ color( Vector4(r, g, b, a) );										}

	void color_hsv(float h, float s, float v)
	{
		float r, g, b;
		image::hsv_to_rgb(h, s, v, r, g, b);
		color_rgb(r, g, b);
	}

	void color_hsva(float h, float s, float v, float a)
	{
		float r, g, b;
		image::hsv_to_rgb(h, s, v, r, g, b);
		color_rgba(r, g, b, a);
	}

	
	
	
	void ignoreViewTransform(bool value)
	{
		shaderDataPerObject.colorFromVertex[1] = value ? 1 : 0;
	}

	
	
	
	void text(const Vector3& worldCoord, const std::string& txt)
	{
		CHECK_INITIALIZED

		text(worldCoord, nm::index2::zeroIndex, txt);
	}

	void text(const Vector3& worldCoord, const nm::index2& screenShift, const std::string& txt)
	{
		CHECK_INITIALIZED
		
		for (auto& viewport : Viewports::viewports)
		{
			nm::index2 screenCoord;
			if (viewport.calculateCamera().screenPos(D3DXVECTOR3(worldCoord.x, worldCoord.y, worldCoord.z), screenCoord))
			{
				text(screenCoord + screenShift, txt);
			}
		}
	}

	void text(const nm::index2& screenCoord, const std::string& txt)
	{
		CHECK_INITIALIZED 
		imgui::label( imgui::Point2(screenCoord.x, screenCoord.y), txt );
	}
	
	void addLine(const nm::index2& start, const nm::index2& end)
	{
		CHECK_INITIALIZED

		unigui::draw::line(start, end, unigui::color(shaderDataPerObject.color.x, shaderDataPerObject.color.y, shaderDataPerObject.color.z, 1.0f));
	}

	void lineRect(const unigui::Rect& screenRect)
	{
 		CHECK_INITIALIZED;
		unigui::draw::linerect(screenRect, unigui::color(shaderDataPerObject.color.x, shaderDataPerObject.color.y, shaderDataPerObject.color.z, 1.0f));
	}

	void rect(const unigui::Rect& screenRect)
	{
		CHECK_INITIALIZED;
		unigui::draw::rect(screenRect, unigui::color(shaderDataPerObject.color.x, shaderDataPerObject.color.y, shaderDataPerObject.color.z, 1.0f));
		unigui::draw::flush();
	}

	void line(const Vector3& start, const Vector3& end)
	{
		CHECK_INITIALIZED
		addLine(start, end);
		endDrawLines();
	}

	void line2(const nm::index2& start, const nm::index2& end)
	{
		CHECK_INITIALIZED
		addLine(start, end);
		endDrawLines();
	}

	void lineloop_impl(const nm::Vector3* points, std::size_t pointsCount)
	{
		CHECK_INITIALIZED

		for (std::size_t i = 1; i < pointsCount; ++i)
		{
			addLine(points[i - 1], points[i]);
		}

		addLine(points[pointsCount - 1], points[0]);		
		endDrawLines();
	}

	void spline(const nm::spline& spln, bool renderVertices)
	{
		nm::Vector3 v1;

		float w1 = 0.1f;
		float w2 = 0.05f;
		float w3 = 0.2f;

		bool bBezier = true;

		auto& vertices = spln.vertices;
		
		if (renderVertices)
		{
			v1 = vertices[0];
			for (unsigned int i = 1; i < vertices.size(); i++)
			{
				color_rgb(1, 1, 1);
				line(v1, vertices[i]);
				line(v1, vertices[i] + nm::Vector3(1, 0, 0));
				line(v1, vertices[i] + nm::Vector3(0, 1, 0));
				line(v1, vertices[i] + nm::Vector3(0, 0, 1));

				v1 = vertices[i];
			}
			if (bBezier)
			{
				for (unsigned int i = 0; i < vertices.size(); i++)
				{
					color_rgb(1, 0, 0);
					line(vertices[i], spln.in[i]);

					color_rgb(0, 1, 0);
					line(vertices[i], spln.out[i]);
				}
			}
		}
		else
		{
			v1 = vertices[0];
			for (unsigned int i = 0; i < vertices.size() - 1; i++)
			{
				for (float mu = 0.0f; mu <= 1.0001f; mu += 0.1f)
				{
					Vector3 v2 = nm::spline::bezier4(vertices[i], spln.out[i], spln.in[i + 1], vertices[i + 1], mu);
					color_rgb(1, 0, 1);
					line(v1, v2);
					v1 = v2;
				}
			}
		}
	}

	void coordinatesGrid(const Base::Interval<int>& bounds, float delimSize)
	{
		CHECK_INITIALIZED

		float scale = delimSize;
		Base::Interval<float> minMax(bounds.minValue * scale, bounds.maxValue * scale);
		
		
		for (int i = bounds.minValue; i <= bounds.maxValue; ++i)
		{
			if(i != 0)
			{
				float si = i * scale;
				float mn = minMax.minValue;
				float mx = minMax.maxValue;

				float clr = (i % 10 == 0) ? 0.5f : 0.4f;

				color_rgb(clr, clr, clr);
				addLine(Vector3(si, 0.0f, mn), Vector3(si, 0.0f, mx));
				addLine(Vector3(mn, 0.0f, si), Vector3(mx, 0.0f, si));
			}
		}
		endDrawLines();	

		color_rgb(1.0f, 1.0f, 1.0f); line(Vector3(0.0f, 0.0f, minMax.minValue), Vector3::zero);
		color_rgb(1.0f, 1.0f, 1.0f); line(Vector3(minMax.minValue, 0.0f, 0.0f), Vector3::zero);
		color_rgb(0.4f, 0.4f, 1.0f); line(Vector3::zero, Vector3(0.0f, 0.0f, minMax.maxValue));
		color_rgb(1.0f, 0.4f, 0.4f); line(Vector3::zero, Vector3(minMax.maxValue, 0.0f, 0.0f));
		
		color_rgb(1.0f, 1.0f, 0.5f);
		text(Vector3(0, 0, 0), 0);		
		
		text(Vector3(minMax.minValue, 0, 0), minMax.minValue );
		text(Vector3(minMax.maxValue, 0, 0), minMax.maxValue );
		text(Vector3(0, 0, minMax.minValue), minMax.minValue );
		text(Vector3(0, 0, minMax.maxValue), minMax.maxValue );		
	}

	void mesh(const MeshType& usermesh, const State& s, float scale)			{ Primitives::Mesh::add(immediateModeEnabled, shaderDataPerObject.color, usermesh, s, scale);	}
	void mesh(const MeshType& usermesh, float scale)							{ mesh(usermesh, State::Identity(), scale); }
	void mesh(const MeshType& usermesh, const Vector3& position, float scale)	{ mesh(usermesh, State::Simple(position), scale); }

	void lines(const MeshType& usermesh, const State& s, float scale)			{ Primitives::LineMesh::add(immediateModeEnabled, shaderDataPerObject.color, usermesh, s, scale); }
	void lines(const MeshType& usermesh, float scale)							{ lines(usermesh, State::Identity(), scale); }
	void lines(const MeshType& usermesh, const Vector3& position, float scale)	{ lines(usermesh, State::Simple(position), scale); }

	
	void box( const State& s, const Vector3& radius)
	{
		CHECK_INITIALIZED
					
		Primitives::Box::add(immediateModeEnabled, shaderDataPerObject.color, s, radius);
	}

	void box(const Vector3& position, const Vector3& size)
	{
		box(State::Simple(position), size);
	}

	void box(const Vector3& position)
	{
		box(position, Vector3::one );
	}

	
	
	
	void cylinder(const State& s, float radius, float height )
	{
		CHECK_INITIALIZED

		Primitives::Cylinder::add(immediateModeEnabled, shaderDataPerObject.color, s, radius, height);
	}

	void cylinder(const Vector3& position, float radius, float height)
	{
		cylinder(State::Simple(position), radius, height);
	}

	void cylinder(const Vector3& position)
	{
		cylinder(position, 1.0f, 1.0f);
	}


	
	void arrow(const Vector3& from, const Vector3& to, float width)
	{
		CHECK_INITIALIZED
		
		float cylinderSize = 0.6f;
		float coneSize = 1.0f - cylinderSize;
		float length = (to - from).Magnitude();

		Vector3 arrowVector = to - from;
		Vector3 cylinderPos = from + arrowVector * cylinderSize;
		Vector3 conePos = to - arrowVector * coneSize;

		float a = Base::MathUtils::GetAzimuthPos(from, to);
		float v = Base::MathUtils::GetVertical(from, to);

		State cylState;
		cylState.position = cylinderPos;
		cylState.SetHPB( Vector3(a, nm::PI_2 - v, 0) );

		State coneState;
		coneState.position = conePos;
		coneState.SetHPB( Vector3(a, nm::PI_2 * 3 - v, 0) );

		cylinder(cylState, width * 0.5f, length * cylinderSize);
		cone(coneState, width * 1.5f, length * coneSize);
	}

	void locator(const State& s, const Vector3& radius)
	{
		CHECK_INITIALIZED

		static const Vector3 bp[8] = 
		{
				Vector3(-1,1,1),
				Vector3(1,1,1),
				Vector3(1,-1,1),
				Vector3(-1,-1,1),
				Vector3(-1,1,-1),
				Vector3(1,1,-1),
				Vector3(1,-1,-1),
				Vector3(-1,-1,-1)
		};

		
		static const int orders[8][4] = 
		{
			{0,1,3,4}, {1,0,5,2}, {2,6,3,1}, {3,0,7,2}, {4,0,5,7}, {5,4,1,6}, {6,5,2,7}, {7,4,3,6}
		};

		
		const float len = 0.2f;
		const float lenfac = nm::SQRT2;

		color( PredefinedColors::white );
		for ( int corner = 0; corner < 8; ++corner )
		{
			const Vector3& axis = bp[ orders[corner][0] ];
			for ( int dir = 1; dir < 4; ++dir )
			{
				const Vector3& farPoint = bp[ orders[corner][dir] ];
				drawTransformedLine( axis, nm::lerp( axis, farPoint, 0.2f ), s, radius );
			}
		}

		
		color( PredefinedColors::gray);
		drawTransformedLine( Vector3::zero, Vector3::xNegAxis * lenfac, s, radius); 
		drawTransformedLine( Vector3::zero, Vector3::yNegAxis * lenfac, s, radius);
		drawTransformedLine( Vector3::zero, Vector3::zNegAxis * lenfac, s, radius);

		
		color( PredefinedColors::red + Vector3(0, 0.5f, 0.5f) );
		drawTransformedLine( Vector3::zero, Vector3::xAxis * lenfac, s, radius );
		

		color( PredefinedColors::green + Vector3(0, 0.5f, 0.5f) );
		drawTransformedLine( Vector3::zero, Vector3::yAxis * lenfac, s, radius );
		

		color( PredefinedColors::blue + Vector3(0, 0.5f, 0.5f) );
		drawTransformedLine( Vector3::zero, Vector3::zAxis * lenfac, s, radius );
		
	}

	void locator(const State& s)
	{
		locator(s, Vector3::one );
	}

	
	
	
	void triangle(const intersections::Triangle& trg)
	{
		triangle(&trg.vertices[0]);
	}

	void triangle(const nm::Vector3* vertices)
	{
		lineloop_impl(vertices, 3);
	}

	void quad(const intersections::Quad& q)
	{
		line(q.vertices[0], q.vertices[1]);
		line(q.vertices[1], q.vertices[2]);
		line(q.vertices[2], q.vertices[3]);
		line(q.vertices[0], q.vertices[3]);
	}

	void ray(const intersections::Ray& r)
	{
		line(r.origin, r.origin + r.direction * 10000);
	}


	
	
	
	void sphere( const State& s, float radius )
	{
		CHECK_INITIALIZED

		Primitives::Sphere::add(immediateModeEnabled, shaderDataPerObject.color, s, radius);
	}

	void sphere(const intersections::Sphere& s)
	{
		sphere(s.center, s.radius);
	}

	void sphere(const Vector3& position, float radius)
	{
		sphere(State::Simple(position), radius);
	}

	void sphere(const Vector3& position)
	{
		sphere(position, 1.0f);
	}

	void sphere(const nm::index3& position)
	{
		sphere(Vector3((float) position.x, (float) position.y, (float) position.z));
	}

	void sphere(const nm::index3& position, float radius)
	{
		sphere(Vector3((float) position.x, (float) position.y, (float) position.z), radius);
	}

	
	
	
	void uniformSphere(const Vector3& pos, float radius, int sphereRings = 6, int sphereSegments = 12, const Vector3& scale = Vector3(1,1,1))
	{
		float deltaRingAngle = (nm::PI / sphereRings);
		float deltaSegAngle = (nm::TwoPI / sphereSegments);

		for (int ring = 0; ring < sphereRings; ring++)
		{
			float r0 = radius * sinf((ring + 0) * deltaRingAngle);
			float r1 = radius * sinf((ring + 1) * deltaRingAngle);
			float y0 = radius * cosf((ring + 0) * deltaRingAngle);
			float y1 = radius * cosf((ring + 1) * deltaRingAngle);

			for (int seg = 0; seg < (sphereSegments + 1); seg++)
			{
				float r0x1 = r0 * cosf((seg + 1) * deltaSegAngle);
				float r0z1 = r0 * sinf((seg + 1) * deltaSegAngle);

				float r1x0 = r1 * cosf(seg * deltaSegAngle);
				float r1z0 = r1 * sinf(seg * deltaSegAngle);

				float r1x1 = r1 * cosf((seg + 1) * deltaSegAngle);
				float r1z1 = r1 * sinf((seg + 1) * deltaSegAngle);

				Vector3 v0 = Vector3(r1x0*scale.x, y1*scale.y, r1z0*scale.z) + pos;
				Vector3 v1 = Vector3(r1x1*scale.x, y1*scale.y, r1z1*scale.z) + pos;
				Vector3 v2 = Vector3(r0x1*scale.x, y0*scale.y, r0z1*scale.z) + pos;

				DebugDrawing::line(v0, v1);
				DebugDrawing::line(v1, v2);
			}
		}
	}

	
	
	
	void cone(const State& s, float radius, float height)
	{
		CHECK_INITIALIZED
		Primitives::Cone::add(immediateModeEnabled, shaderDataPerObject.color, s, radius, height);
	}


	void cone(const Vector3& position, float radius, float height)
	{
		cone(State::Simple(position), radius, height);
	}

	void cone(const Vector3& position)
	{
		cone(position, 1.0f, 1.0f);
	}

	void cone(const Vector3& locus, const Vector3& direction, float radius)
	{
		Vector3 to = locus;
		Vector3 from = locus + direction;

		float a = Base::MathUtils::GetAzimuthPos(from, to);
		float v = Base::MathUtils::GetVertical(from, to);

		State coneState;
		coneState.position = from;
		coneState.SetHPB( Vector3(a, nm::PI_2 * 3 - v, 0) );

		cone(coneState, radius, nm::magnitude(direction));
	}

	
	
	
	void wireBox(const State& s, float extents) { wireBox(s, Vector3(extents, extents, extents)); }
	void wireBox(const State& s, const Vector3& extents)
	{
		Vector3 t00, t01, t10, t11;
		Vector3 b00, b01, b10, b11;

		Vector3 x = s.orientation.GetXAxis() * extents.x;
		Vector3 y = s.orientation.GetYAxis() * extents.y;
		Vector3 z = s.orientation.GetZAxis() * extents.z;

		Vector3 p = s.position;

		t00 = p - x + y - z;
		t01 = p - x + y + z;
		t10 = p + x + y - z;
		t11 = p + x + y + z;

		b00 = p - x - y - z;
		b01 = p - x - y + z;
		b10 = p + x - y - z;
		b11 = p + x - y + z;

		addLine(t00, t10);
		addLine(t10, t11);
		addLine(t11, t01);
		addLine(t01, t00);

		addLine(b00, b10);
		addLine(b10, b11);
		addLine(b11, b01);
		addLine(b01, b00);

		addLine(b00, t00);
		addLine(b10, t10);
		addLine(b11, t11);
		addLine(b01, t01);

		endDrawLines();
	}

	void wireBox(const intersections::OBB& obb)
	{
		Vector3 ext_axis0 = obb.extent[0] * obb.axis[0];
		Vector3 ext_axis1 = obb.extent[1] * obb.axis[1];
		Vector3 ext_axis2 = obb.extent[2] * obb.axis[2];

		Vector3 vertex[8];

		vertex[0] = obb.center - ext_axis0 - ext_axis1 - ext_axis2;
		vertex[1] = obb.center + ext_axis0 - ext_axis1 - ext_axis2;
		vertex[2] = obb.center + ext_axis0 + ext_axis1 - ext_axis2;
		vertex[3] = obb.center - ext_axis0 + ext_axis1 - ext_axis2;
		vertex[4] = obb.center - ext_axis0 - ext_axis1 + ext_axis2;
		vertex[5] = obb.center + ext_axis0 - ext_axis1 + ext_axis2;
		vertex[6] = obb.center + ext_axis0 + ext_axis1 + ext_axis2;
		vertex[7] = obb.center - ext_axis0 + ext_axis1 + ext_axis2;

		addLine(vertex[0], vertex[1]);
		addLine(vertex[1], vertex[2]);
		addLine(vertex[2], vertex[3]);
		addLine(vertex[3], vertex[0]);

		addLine(vertex[4], vertex[5]);
		addLine(vertex[5], vertex[6]);
		addLine(vertex[6], vertex[7]);
		addLine(vertex[7], vertex[4]);

		addLine(vertex[0], vertex[4]);
		addLine(vertex[1], vertex[5]);
		addLine(vertex[2], vertex[6]);
		addLine(vertex[3], vertex[7]);

		endDrawLines();
	}

	void wireBox(const intersections::AABB& aabb)
	{
		 wireBox(intersections::OBB(aabb));
	}

	void wireBox(const Vector3& position, float extents)
	{
		wireBox(State::Simple(position), extents);
	}

	void wireBox(const Vector3& position, const Vector3& extents)
	{
		wireBox(State::Simple(position), extents);
	}

	void wireBox(const Vector3& position)
	{
		wireBox(position, Vector3::one);
	}

	void wirePrysm(const Vector3* base, int baseCount, const Vector3& edge)
	{
		CHECK_INITIALIZED;

		int maxVal = baseCount - 1;
		for (int i = 0; i <= maxVal; ++i)
		{
			const Vector3& b0 = base[i];
			int next = nm::ring_clamp(i + 1, maxVal);
			const Vector3& b1 = base[next];

			auto b01 = b0 + edge;
			auto b11 = b1 + edge;

			addLine(b0, b1);
			addLine(b01, b11);
			addLine(b0, b01);
		}
		endDrawLines();	
	}

	
	
	
	void frustum(const CameraSystem::Params& cam)
	{
		const State& s = cam.getState();

		float farPlane = cam.farPlane();
		float nearPlane = cam.nearPlane();
		float hheight = tan( cam.getFovV() * 0.5f); 
		float hwidth = hheight * cam.getAspect();

		Vector3 x; s.orientation.GetXAxis(x);
		Vector3 y; s.orientation.GetYAxis(y);
		Vector3 z; s.orientation.GetZAxis(z);
		
		nm::Vector3 right;
		nm::Vector3 top;
		nm::Vector3 center;

		right	=	hwidth	* farPlane * x;
		top		=	hheight	* farPlane * y;
		center	=	s.position + farPlane * z;

		nm::Vector3 ftl = center - right + top; 
		nm::Vector3 ftr = center + right + top; 
		nm::Vector3 fbl = center - right - top; 
		nm::Vector3 fbr = center + right - top; 

		right	=	hwidth	* nearPlane * x;
		top		=	hheight	* nearPlane * y;
		center	=	s.position + nearPlane * z;

		nm::Vector3 ntl = center - right + top; 
		nm::Vector3 ntr = center + right + top; 
		nm::Vector3 nbl = center - right - top; 
		nm::Vector3 nbr = center + right - top; 

		nm::Vector4 clr = color();

		lineloop(ftl, ftr, fbr, fbl);
		lineloop(ntl, ntr, nbr, nbl);
		line(ntl, ftl);
		line(ntr, ftr);
		line(nbl, fbl);
		line(nbr, fbr);

		color(clr*0.5f);
		line(ntl, s.position);
		line(ntr, s.position);
		line(nbl, s.position);
		line(nbr, s.position);

		color(clr);
	} 

	
	
	
	void camera(const State& cam, float size)
	{
		const Vector3 pos(0,0,0);
		float coneHeight = 0.5f * size;

		float coneRadius		= coneHeight * 0.5f;
		float boxZExt			= coneHeight * 1.1f;
		float boxYExt			= coneRadius * 1.2f;
		float boxXExt			= coneRadius;
		float cylinderRadius	= boxZExt*0.5f;
		float cylinderHeight	= boxXExt*0.9f;

		State coneS		 = State::Simple(pos);
		State boxS		 = State::Simple(pos);	
		State cylinderS0 = State::Simple(pos);
		State cylinderS1 = State::Simple(pos);

		
		coneS.SetHPB(Vector3(0, nm::PI_2, 0));

		boxS.position.z -= coneHeight + boxZExt;

		cylinderS0.SetHPB(Vector3(0, 0, nm::PI_2));
		cylinderS0.position = boxS.position + Vector3(cylinderHeight * 0.5f, boxYExt + cylinderRadius, -cylinderRadius);

		cylinderS1 = cylinderS0;
		cylinderS1.position.z += 2.0f * cylinderRadius;

		coneS		= cam * coneS;
		boxS		= cam * boxS;
		cylinderS0	= cam * cylinderS0;
		cylinderS1	= cam * cylinderS1;

		cone(coneS, coneRadius, coneHeight);
		box(boxS, Vector3(boxXExt, boxYExt, boxZExt));
		cylinder(cylinderS0, cylinderRadius, cylinderHeight);
		cylinder(cylinderS1, cylinderRadius, cylinderHeight);
	}

	void camera(const CameraSystem::Params& cam, float size, bool drawFrustum)
	{
		camera(cam.getState(), size);

		if(drawFrustum)
		{
			frustum(cam);
		}		
	}

	void camera(const CameraSystem::UserCameraBase& cam, float size, bool drawFrustum)
	{
		camera(cam.params, size, drawFrustum);
	}
}



// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated 
// documentation files (the "Software"), to deal in the Software without restriction, including without limitation 
// the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, 
// and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all copies or substantial portions 
// of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED 
// TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL 
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF 
// CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
// DEALINGS IN THE SOFTWARE.