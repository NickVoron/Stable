///////////////////////////////////////////////////////////////////////  
//  SpeedTree Effect File
//
//  *** INTERACTIVE DATA VISUALIZATION (IDV) PROPRIETARY INFORMATION ***
//
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Interactive Data Visualization and may
//  not be copied or disclosed except in accordance with the terms of
//  that agreement.
//
//      Copyright (c) 2003-6 IDV, Inc.
//      All Rights Reserved.
//
//      IDV, Inc.
//      Web: http://www.idvinc.com	    
//		   
//  *** Release Version 4.0 ***
//
//  These shaders were written to be used as HLSL FX or CgFX files for the
//  following platforms/APIs:
//      - Windows DirectX 9.0c
//      - Windows OpenGL 1.4+
//      - Xbox(R) 360
//      - PLAYSTATION(R)3
//
//  SpeedTree.fx is the main fx files that's compiled by the application.  It
//  includes several other fx files that define shaders for each of the geometry
//  types composing a SpeedTree model.
//
//  As of the initial release of SpeedTreeRT 4.0, Cg 1.5 for the PC was still
//  in a beta release and still had a few critical bugs that prohibited it
//  for being used.  Cg 1.4 is used instead.  This earlier version does not
//  support some of the same keywords as HLSL FX files, especially in the
//  compile statements and sampler declaration.  As a result, some of the
//  commands are #ifdef'd around the macro IDV_OPENGL.
//
//  There are several macros used throughout these fx files that are defined
//  at runtime by the application (they're passed in with the compile command):
//
//  IDV_OPENGL
//  Defined for Windows PC and PLAYSTATION3 builds.  When not defined, Windows
//  DirectX 9.0c and Xbox 360 builds are assumed.  It is used in most of the
//  fx files.
//
//  PS3
//  Used to specify the unique vertex and pixel shader build types
//  (sce_vp_TypeC and sce_fp_TypeC).
//
//  VS_COMPILE_COMMAND and PS_COMPILE_COMMAND
//  Three of the four platforms have unique shader compile targets and 
//  commands. The complete compile commands, including the targets, are stored 
//  in these macros.  They're used in most of the fx files.
//
//  BRANCH_DETAIL_LAYER
//  The sample branch shaders include support for three texture layers:  diffuse
//  map, normal map, and detail map.  Support of the detail map layer is optional 
//  and is enabled by defining this macro.  It is used only in Branch.fx
//
//  USE_FOG
//  The use of fog in a pixel shader is a non-trivial expense.  It can be toggled
//  using this macro.  It is used in most of the fx files.
//
//  NUM_LEAF_ANGLES
//  Each of the leaves in a particular tree model point to one of several
//  different rock and rustle angles.  This allows independent wind behavior for
//  the leaves, leading to a more realistic wind effect.  This macro is set to 
//  define the number of leaf rock/rustle angles that can be uploaded.  It is
//  used only in Leaf.fx.
//
//  NUM_360_IMAGES
//  This macro defines maximum number of individual 360-degree images that can
//  uploaded per tree.  It is used only in Billboard.fx
//
//  NUM_WIND_MATRICES
//  Each main branch, and its corresponding children (smaller branches, leaves) 
//  refer to a single wind matrix to define its sway behavor.  This macro defines
//  the total number of wind matrices uploaded per frame. It is used in Utility.fx
//  where the wind function is housed, which is called by Branch.fx, Frond.fx, and
//  Leaf.fx.
//
//	SUPPORT_360_BILLBOARDS
//	Most applications should use this macro, which enables the billboard vertex
//	shaders BillboardVS1 and BillboardVS2 to compute the fade alpha values during
//  360 image transitions.  Disabling the macro will fade the shaders only compute
//  alpha values for the fade between 3D and billboard.
//
//  Typical command-line compile command:
//  cgc -profile vp30 -DNUM_WIND_MATRICES=7 -DMAX_NUM_LEAF_ANGLES=20 -DMAX_NUM_360_IMAGES=18 -entry LeafVS SpeedTree.fx
									  
#define NUM_WIND_MATRICES 7			  
#define MAX_NUM_LEAF_ANGLES 20
#define MAX_NUM_360_IMAGES 18
#define NUM_360_IMAGES 18				 
#define UPVECTOR_POS_Y			  
#define SUPPORT_360_BILLBOARDS


// textures
texture     leavesDiffuseMap;
texture     frondsDiffuseMap;
texture     billboardDiffuseMap;
texture     billboardNormalMap;

// wind
float       g_fWindMatrixOffset;                    // keeps two instances of the same tree model from using the same wind matrix (range: [0,NUM_WIND_MATRICES])
float4x4    g_amWindMatrices[NUM_WIND_MATRICES];    // houses all of the wind matrices shared by all geometry types
float2      g_vSelfShadowMotion;                    // offsets the self-shadow coordinates based on wind strength

// lighting & materials
float4      g_vMaterialDiffuse;                     // active material's diffuse color (set differently for branches, fronds, leaves, and billboards)
float       g_fDiffuseScale;                        // mirrors the diffuse scalar value in CAD, used to help combat the darkening effect of multiple texture layers
float4      g_vMaterialAmbient;                     // active material's ambient color (set differently for branches, fronds, leaves, and billboards)
float       g_fAmbientScale;                        // mirrors the ambient scalar value in CAD, used to help combat the darkening effect of multiple texture layers
float       g_fLeafLightingAdjust;

// other
float2      g_vCameraAngles;                        // shared by Billboard.fx and Leaf.fx - stores camera azimuth and pitch for billboarding computations               

int renderBillboard = 1;
int colorPass = true;

sampler2D samLeavesMap = sampler_state
{
    Texture = <leavesDiffuseMap>;
	MaxAnisotropy = 16;	MinFilter = ANISOTROPIC;	MagFilter = LINEAR;	MipFilter = LINEAR;
};

sampler2D samFrondsMap = sampler_state
{
    Texture = <frondsDiffuseMap>;
	MaxAnisotropy = 16;	MinFilter = ANISOTROPIC;	MagFilter = LINEAR;	MipFilter = LINEAR;
};

sampler2D samBillboardDiffuseMap = sampler_state
{
    Texture = <billboardDiffuseMap>;
	MaxAnisotropy = 16;	MinFilter = ANISOTROPIC;	MagFilter = LINEAR;	MipFilter = LINEAR;

};

sampler2D samBillboardNormalMap = sampler_state
{
    Texture = <billboardNormalMap>;
	MaxAnisotropy = 16;	MinFilter = ANISOTROPIC;	MagFilter = LINEAR;	MipFilter = LINEAR;
};

#include "globalConstants.fx"
#include "utility.fx"
#include "stBranch.fx"
#include "stLeaf.fx"
#include "stFrond.fx"
#include "stBillboard.fx"

