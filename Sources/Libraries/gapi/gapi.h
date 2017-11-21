#pragma once

#include "primitiveTopologyDX9.h"
#include "primitiveTopologyDX11.h"
#include "primitiveTopologyGL.h"
#include "shaders.h"
#include "samplers.h"
#include "rasterizerState.h"

namespace gapi
{
	void initialize(Id id);
	bool initialized(Id id);
}