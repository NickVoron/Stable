float lightEvalIntensityDiff(float3 r, float3 n, float3 dir, float3 att, float sc0, float sc1)
{
	float3 ra; ra.z = dot(r, r); ra.y = sqrt(ra.z); ra.x = 1.0f;
	return saturate((dot(dir, r)/ra.y + sc0) * sc1) * max(-dot(n, r), 0.0f) / (ra.y * dot(att, ra));
}

float lightDirectDiff(float3 d, float3 n)
{
	return max(-dot(n, d), 0.0f);
}

float lightPointDiff(float3 r, float3 n, float3 att)
{
	float3 ra; ra.z = dot(r, r); ra.y = sqrt(ra.z); ra.x = 1.0f;
	return max(-dot(n, r), 0.0f) / (ra.y * dot(att, ra));
}

float lightSpotDiff(float3 r, float3 d, float3 n, float3 att, float sc0, float sc1)
{
	float3 ra; ra.z = dot(r, r); ra.y = sqrt(ra.z); ra.x = 1.0f;
	return saturate((dot(d, r)/ra.y + sc0) * sc1) * max(-dot(n, r), 0.0f) / (ra.y * dot(att, ra));
}

float lightDirectSpec(float3 d, float3 n, float3 i, float scp)
{
	return pow(saturate(dot(reflect(i, n), d)), scp);
}

float lightPointSpec(float3 r, float3 n, float3 att, float3 i, float scp)
{
	float3 ra; ra.z = dot(r, r); ra.y = sqrt(ra.z); ra.x = 1.0f;
	return pow(saturate(dot(reflect(i, n), r) / ra.y), scp) / dot(att, ra);
}

float lightSpotSpec(float3 r, float3 d, float3 n, float3 att, float sc0, float sc1, float3 i, float scp)
{
	float3 ra; ra.z = dot(r, r); ra.y = sqrt(ra.z); ra.x = 1.0f;
	return pow(saturate(dot(reflect(i, n), r) / ra.y), scp) * saturate((dot(d, r)/ra.y + sc0) * sc1) / dot(att, ra);
}
