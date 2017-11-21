#include "affector.h"

namespace CameraComponents
{

void AffectorBase::linker()
{
	link(paramHolder);
}

void AffectorBase::finalize()
{
	//paramHolder->camera.affectorsSet.push_back(affector);
}

}