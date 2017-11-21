#include "quad.h"

namespace intersections
{
	Quad Quad::create(const State& state, const Vector2& halfSize)
	{
		Vector3 pos = state.position;

		Quad q;

		q.vertices[0] = pos + state.orientation.Transform(Vector3(-halfSize.x, -halfSize.y, 0));
		q.vertices[1] = pos + state.orientation.Transform(Vector3(halfSize.x, -halfSize.y, 0));
		q.vertices[2] = pos + state.orientation.Transform(Vector3(halfSize.x, halfSize.y, 0));
		q.vertices[3] = pos + state.orientation.Transform(Vector3(-halfSize.x, halfSize.y, 0));

		return q;
	}

	void Quad::transform(const State& state, float scale)
	{
		for (int i = 0; i < VERTICES_COUNT; ++i)
		{
			vertices[i] = state.GetLocalVector(vertices[i] * scale);
		}

	}


}
