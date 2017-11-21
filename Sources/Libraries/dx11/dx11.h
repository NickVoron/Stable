#pragma once

#include <d3dx11.h>

#include "inputLayout.h"
#include "shaders.h"

#include "vertexBuffer.h"
#include "indexBuffer.h"
#include "constantBuffer.h"
#include "rasterizerState.h"
#include "blendState.h"
#include "shaderResource.h"
#include "samplers.h"
#include "renderTargets.h"

#include "formatInfo.h"
#include "io.h"
#include "renderer.h"
#include "instancedRenderer.h"
#include "device.h"
#include "drawCaller.h"

namespace dx11
{

  	void DeviceCreate();
  	void DeviceRelease();
}