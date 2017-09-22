/**
 * Precomputed Atmospheric Scattering
 * Copyright (c) 2008 INRIA
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holders nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * Author: Eric Bruneton
 */

// copies deltaS into S (line 5 in algorithm 4.1)

#include "common.fx"

float r;
float4 dhdH;
int layer;

void main(DEFAULT_PS_INPUT input, out float4 gl_FragColor : COLOR )
{
    float3 uvw = float3(input.gl_FragCoord.xy, float(layer) + 0.5) / float3(int3(RES_MU_S * RES_NU, RES_MU, RES_R));
    float4 ray = tex3Dlod(deltaSRSampler, float4(uvw, 0));
    float4 mie = tex3Dlod(deltaSMSampler, float4(uvw, 0));
    gl_FragColor = float4(ray.rgb, mie.r); // store only red component of single Mie scattering (cf. "Angular precision")
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
technique Render 
{
	pass P0
	{	 
		ZEnable = false;
		ZWriteEnable = false;
		CullMode = None;

    	PixelShader = compile ps_3_0 main();
	}
}





