#pragma once

#include "camera.h"
#include "screenMouseHelper.h"


namespace AppModules
{

class CameraCreator : public ModuleBase<CameraCreator, Loki::MakeTypelist<Camera, ScreenMouseHelper>>
{
public:	
	template<class Params>  void process(Params& p)	{ update(p.mouseRay); }

	void update(const intersections::Ray& mouseRay);
};

}