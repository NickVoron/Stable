#include "position.h"


Vector3& QuaterionToHPB(const Quaternion& q, Vector3& hpb)
{
	hpb = Base::MathUtils::QuaterionToHPB(q);
	return hpb;
}

Quaternion& HPBtoQuaternion(const Vector3& hpb, Quaternion& q)
{
	q = Base::MathUtils::HPBtoQuaternion(hpb);
	return q;
}

void Position::Resource::properties(Position& component)
{
	property("position", component.state.position);
	property("hpb", component.state.orientation, &HPBtoQuaternion, &QuaterionToHPB);
}

namespace properties_inheritance
{
	void inherite(Position& parent, Position& child)
	{
		child.state += parent.state;
	}
}

