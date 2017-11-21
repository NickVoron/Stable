#pragma once

namespace DebugDrawing
{
	struct DrawableObject;

	DrawableObject* endDrawLinesImpl();

	enum ShaderMode
	{
		INTERNAL_DEFAULT_SHADER,
		INTERNAL_SOLID_SHADER,

		INTERNAL_SHADERS_COUNT,

		EXTERNAL_SHADER,
	};

	typedef Geometry::Vertex<Geometry::POS3, Geometry::NORMAL, Geometry::DIFFUSE>	VertexType;
	typedef unsigned int									IndexType;
	typedef Geometry::Mesh<VertexType, IndexType>			MeshType;



	namespace Primitives
	{

		struct Line;
		struct Box;
		struct Sphere;
		struct Cylinder;
		struct Cone;
		struct Mesh;
		struct LineMesh;

		DrawableObject* draw(const Primitives::Line& p);
		DrawableObject* draw(const Primitives::Box& p);
		DrawableObject* draw(const Primitives::Sphere& p);
		DrawableObject* draw(const Primitives::Cylinder& p);
		DrawableObject* draw(const Primitives::Cone& p);
		DrawableObject* draw(const Primitives::Mesh& p);
		DrawableObject* draw(const Primitives::LineMesh& p);

		struct PimitiveBase { 	Vector4 color;	};
		struct Transform	{	State state;	};
		
		struct DrawableListBase : public Base::IntrusiveList<DrawableListBase>::Node
		{
			virtual void draw(std::list<DrawableObject*>& objects) const = 0;
			virtual void reset() = 0;
		};

		struct DrawableLists : public Base::IntrusiveList<DrawableListBase>
		{
			void draw(std::list<DrawableObject*>& objects) const;
			void reset();

			static DrawableLists& get();
		};

		template<class PT>
		struct DrawableList : public std::vector<PT>, public DrawableListBase
		{
			template<class... P0>			
			void add(bool immediate, const Vector4& color,  const P0&... p0)					
			{ 
				if (immediate)
				{
					Primitives::draw(PT(color, p0...));
				}
				else
				{
					this->push_back(PT(color, p0...));
				}
			}

			DrawableList()
			{
				static bool doublicate = false;
				ENFORCE(!doublicate);
				doublicate = true;
				DrawableLists::get().push_back(*this);
			}

			virtual void draw(std::list<DrawableObject*>& objects) const
			{	
				for(const PT& d : *this)	
				{ 
					DrawableObject* obj = d.draw();
					if (obj)
					{
						objects.push_back(obj);
					}					
				} 


			}
			virtual void reset()			{	this->clear();	}
		};

		template<class RE>
		struct RenderElement
		{
			DrawableObject* draw() const { return Primitives::draw(*(RE*)this); }
			
			static DrawableList<RE>& drawables() {	static DrawableList<RE> list;	return list; }

			template<class P0, class P1>			static void add(bool immediate, const Vector4& color,  const P0& p0, const P1& p1)				{ drawables().add(immediate, color,  p0, p1); }
			template<class P0, class P1, class P2>	static void add(bool immediate, const Vector4& color, const P0& p0, const P1& p1, const P2& p2) { drawables().add(immediate, color,  p0, p1, p2); }
		};
		
		struct Line : public PimitiveBase, public RenderElement<Line>
		{
			Line(const Vector4& clr, const Vector3& p0, const Vector3& p1)	{ points[0] = p0; points[1] = p1; color = clr; }
			Vector3 points[2];
		};

		struct Box : public PimitiveBase, public Transform, public RenderElement<Box>
		{
			Box(const Vector4& clr, const State& s, const Vector3& ext):extents(ext) { state = s; color = clr; }
			Vector3 extents;
		};

		struct Sphere : public PimitiveBase, public Transform, public RenderElement<Sphere>
		{
			Sphere(const Vector4& clr, const State& s, float r):radius(r) { state = s; color = clr; }
			float radius;
		};

		struct Cylinder : public PimitiveBase, public Transform, public RenderElement<Cylinder>
		{
			Cylinder(const Vector4& clr, const State& s, float r, float h):radius(r), height(h) { state = s; color = clr; }
			float radius; 
			float height;
		};

		struct Cone : public PimitiveBase, public Transform, public RenderElement<Cone>
		{
			Cone(const Vector4& clr, const State& s, float r, float h):radius(r), height(h) { state = s; color = clr; }
			float radius; 
			float height;
		};

		struct LineMesh : public PimitiveBase, public Transform, public RenderElement<LineMesh>
		{
			LineMesh(const Vector4& clr, const MeshType& msh, const State& s, float scl) :mesh(msh) { state = s; color = clr; scale = scl; }
			MeshType mesh;
			float scale;
		};

		struct Mesh : public PimitiveBase, public Transform, public RenderElement<Mesh>
		{
			Mesh(const Vector4& clr, const MeshType& msh, const State& s, float scl) :mesh(msh) { state = s; color = clr; scale = scl; }
			MeshType mesh;
			float scale;
		};
	}
	
}