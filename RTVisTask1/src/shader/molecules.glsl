-- Vertex

#extension GL_ARB_explicit_attrib_location : enable

// Input:
in vec3 vs_in_positionW;
in vec3 vs_in_color;
in float vs_in_radius;

// Output:
out vec3 vs_out_positionW;
out vec3 vs_out_color;
out float vs_out_radius;

void main()
{
	// Output variables:
	vs_out_positionW = vs_in_positionW;
	vs_out_color = vs_in_color;
	vs_out_radius = vs_in_radius;
}

-- Geometry

// Specify input and output primitives:
layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

// Uniforms:
uniform mat4 u_viewMatrix;
uniform mat4 u_inverseViewMatrix;
uniform mat4 u_projectionMatrix;
uniform mat4 u_viewProjectionMatrix;
uniform vec3 u_eyePositionW;

// Input:
in vec3 vs_out_positionW[];
in vec3 vs_out_color[];
in float vs_out_radius[];

// Output:
out vec3 gs_out_positionW;
flat out vec3 gs_out_color;
flat out float gs_out_radiusV;
flat out vec3 gs_out_centerV;
out vec2 gs_out_textureCoordinates;

void main()
{
	vec3 centerV = (u_viewMatrix * vec4(vs_out_positionW[0], 1.0f)).xyz;
	float radiusV = vs_out_radius[0];
	
	// Calculate the local coordinate system:
	vec3 up = vec3(0.0f, 1.0f, 0.0f);
	vec3 forward = normalize(vec3(0.0f, 0.0f, 0.0f) - centerV);
	vec3 right = cross(up, forward);

	// Compute the position of the vertices of the billboard:
	vec4 verticesV[4];
	verticesV[0] = vec4(centerV - up * radiusV - right * radiusV, 1.0f);
	verticesV[1] = vec4(centerV - up * radiusV + right * radiusV, 1.0f);
	verticesV[2] = vec4(centerV + up * radiusV - right * radiusV, 1.0f);
	verticesV[3] = vec4(centerV + up * radiusV + right * radiusV, 1.0f);

	vec2 textureCoordinates[4] =
	{
		vec2(-1.0f, -1.0f),
		vec2(1.0f, -1.0f),
		vec2(-1.0f, 1.0f),
		vec2(1.0f, 1.0f)
	};

	// Emit vertices:
	for (int i = 0; i < 4; ++i)
	{
		gl_Position = u_projectionMatrix * verticesV[i];
		gs_out_positionW = (u_inverseViewMatrix * verticesV[i]).xyz;
		gs_out_color = vs_out_color[0];
		gs_out_radiusV = radiusV;
		gs_out_centerV = centerV;
		gs_out_textureCoordinates = textureCoordinates[i];
		EmitVertex();
	}
	EndPrimitive();
} 

--Fragment

// Uniforms:
uniform vec3 u_eyePositionW;

// Input:
in vec3 gs_out_positionW;
flat in vec3 gs_out_color;
flat in float gs_out_radiusV;
flat in vec3 gs_out_centerV;
in vec2 gs_out_textureCoordinates;

// Output:
out vec4 fs_out_color;

#include "LightingUtils.glsli"

void main()
{
	vec2 centerUV = vec2(0.0f, 0.0f);
	float radiusUV = 1.0f;

	vec2 fromCenterUV = gs_out_textureCoordinates - centerUV;
	float fromCenterDistanceUV = length(fromCenterUV);
	if (fromCenterDistanceUV > radiusUV)
		discard;

	float depthOffset = sqrt(1.0f - fromCenterDistanceUV * fromCenterDistanceUV);
	vec3 positionUV = vec3(gs_out_textureCoordinates, depthOffset);

	// TODO normal in world space
	vec3 normal = normalize(positionUV - centerUV);
	//vec3 normal = vec3(1.0f, 0.0f, 0.0f);

	Light lights[MAX_NUM_LIGHTS];
	lights[0].Strength = vec3(0.8f, 0.8f, 0.8f);
	lights[0].FalloffStart = 50.0f;
	lights[0].FalloffEnd = 200.0f;
	lights[0].Position = vec3(0.0f, 0.0f, 20.0f);

	Material material;
	material.DiffuseAlbedo = vec4(gs_out_color, 1.0f);
	material.FresnelR0 = vec3(0.95f, 0.93f, 0.88f);
	material.Shininess = 32.0f;

	vec4 lightIntensity = ComputeLighting(lights, material, gs_out_positionW, normal, u_eyePositionW);
	vec4 ambientIntensity = vec4(0.05f, 0.05f, 0.05f, 0.0f);

	vec4 color = lightIntensity + ambientIntensity;
	color.a = 1.0f;

	fs_out_color = color;

	//vec3 fromCenterVector = gs_out_positionV - gs_out_centerV;
	//if (dot(fromCenterVector, fromCenterVector) > gs_out_radiusV * gs_out_radiusV)
	//		discard;

	//fs_out_color = vec4(gs_out_color, 1.0f);
}
