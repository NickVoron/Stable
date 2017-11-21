#include "boxConstraint.h"

namespace CameraComponents
{

void BoxConstraint::Resource::properties(BoxConstraint& component)
{
	property("rangeX", component.camAffector.data.x);
	property("rangeY", component.camAffector.data.y);
	property("rangeZ", component.camAffector.data.z);
	property("enableX", component.camAffector.data.enabledX);
	property("enableY", component.camAffector.data.enabledY);
	property("enableZ", component.camAffector.data.enabledZ);
}

}
