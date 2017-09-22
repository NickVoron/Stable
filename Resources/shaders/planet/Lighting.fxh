//Light struct that represents a Point, Spot, or Directional
//light. Point lights are with a 180 degree inner/outer angle,
//and directional lights have their position's w = 1.0 .
struct Light {
	//Color properties
	float3 Ambient;
	float3 Diffuse;
	float3 Specular;
	
	//Attenuation properties
	bool Attenuate;
	float Constant;
	float Linear;
	float Quadratic;
	
	//Positional (in world space) properties
	float4 Position; //Note: w = 1 means direction is used
	float3 Direction;
	float InnerAngle;
	float OuterAngle;
};

//The resulting light color contribution from a single light,
//these are generally modulated by other colors like textures.
struct LightResult {
	float3 Ambient;
	float3 Diffuse;
	float3 Specular;
};

struct Material {
	float3 Ambient;
	float3 Diffuse;
	float3 Specular;
	float Shininess;
};

float3 Expand(float3 normal) {
	return (normal * 2) - 1;
}

float4 Expand(float4 normal) {
	return (normal * 2) - 1;
}

//Compute the attenuation factor of a light based off of a world space
//position, if the light is set to attenuate or not.
float ComputeAttenuation(float3 position, Light light){
	if(light.Attenuate == true){
		float d = distance(position, light.Position.xyz);
		return 1 / (light.Constant + (light.Linear*d) + (light.Quadratic * d * d));
	} else {
		return 1;
	}
}

//Compute the spot effect used by point/spot lights
//Point and Directional lights will have their angles be 180 (1 - 180/180 = 0)
//Otherwise we use the angles to create the spot light concentration and smooth
//it out 
float SpotEffect(float3 position, Light light){
	if(light.InnerAngle == 0 && light.OuterAngle == 0){
		return 1;
	} else {
		float3 V = normalize(position - light.Position.xyz);
		return smoothstep(light.OuterAngle, light.InnerAngle, dot(V, light.Direction));
	}
}

//Compute a single light - taking into account the Material Properties.
//Requires the vertex position in world space, the normal in world space, and the
//view vector V in world space.
LightResult ComputeSingleLight(Light light, float3 position, float3 normal, float3 V, Material mat){
	//Calculate ambient contribution
	float3 ambient = light.Ambient * mat.Ambient;
	
	//Calculate diffuse contribution
	
	//Direction to the light source
	float3 L;
	if(light.Position.w == 1){
		L = -light.Direction; //Should already by normalized by the CPU
	} else {
		L = normalize(light.Position.xyz - position);
	}
	float NDotL = max(dot(normal,L),0);
	float3 diffuse = NDotL  * light.Diffuse * mat.Diffuse;
	
	//Calculate specular contribution
	
	//Vector halfway between L and V
	float3 H = normalize(L + V);

	float ZeroNDotL = step(0, NDotL);

	float NDotH = pow(max(dot(normal,H),0) * ZeroNDotL, mat.Shininess);

	float3 specular = NDotH * light.Specular * mat.Specular;
	
	//Return result
	LightResult result;
	result.Ambient = ambient;
	result.Diffuse = diffuse;
	result.Specular = specular;
	return result;
}