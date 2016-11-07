--Vertex

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

--Geometry

// Specify input and output primitives:
layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

// Uniforms:
uniform float u_aspectRatio;
uniform mat4 u_viewProjectionMatrix;
uniform mat4 u_inverseViewProjectionMatrix;
uniform float u_viewMatrixDeterminantCubicRoot;

// Input:
in vec3 vs_out_positionW[];
in vec3 vs_out_color[];
in float vs_out_radius[];

// Output:
out vec3 gs_out_positionW;
flat out vec3 gs_out_color;
flat out float gs_out_radius;
out vec2 gs_out_textureCoordinates;

void main()
{
	vec3 positionW = vs_out_positionW[0];
	float radius = vs_out_radius[0];

	// Calculate the center by transform vertex position from world space to clip space:
	vec4 centerH = u_viewProjectionMatrix * vec4(positionW, 1.0f);

	// Each vertex will have its own texture coordinates:
	vec2 textureCoordinates[4] =
	{
		vec2(-1.0f, -1.0f),
		vec2(1.0f, -1.0f),
		vec2(-1.0f, 1.0f),
		vec2(1.0f, 1.0f)
	};

	// Create 4 vertices which represent the billboard quad.
	// This vertices are displaced in screen coordinates after projection in order to produce a screen aligned quad.
	vec4 verticesH[4];
	{
		// Calculate displacement scalar by multiplying the radius of the sphere by a global scalar value:
		float displacementScalar = radius * u_viewMatrixDeterminantCubicRoot;
		for (int i = 0; i < 4; ++i)
		{
			// Calculate displacement of the current vertex:
			vec2 displacement = vec2(textureCoordinates[i].x * displacementScalar, textureCoordinates[i].y * displacementScalar);
			displacement.y *= u_aspectRatio;

			// Calculate the position of the displaced vertex:
			verticesH[i] = vec4(centerH.x + displacement.x, centerH.y + displacement.y, centerH.z, centerH.w);
		}
	}

	// Emit vertices:
	for (int i = 0; i < 4; ++i)
	{
		gl_Position = verticesH[i];
		gs_out_positionW = (u_inverseViewProjectionMatrix * verticesH[i]).xyz;
		gs_out_color = vs_out_color[0];
		gs_out_radius = radius;
		gs_out_textureCoordinates = textureCoordinates[i];
		EmitVertex();
	}
	EndPrimitive();
}

--Fragment

// Uniforms:
uniform mat4 u_inverseViewMatrix;
uniform mat4 u_viewProjectionMatrix;
uniform vec3 u_eyePositionW;
uniform vec3 u_materialFresnelR0;
uniform float u_materialShininess;

// Input:
in vec3 gs_out_positionW;
flat in vec3 gs_out_color;
flat in float gs_out_radius;
in vec2 gs_out_textureCoordinates;

// Output:
out vec4 fs_out_color;

#include "LightingUtils.glsli"

vec3 MappingFromUVToXYZ(vec2 uv)
{
	vec3 xyz;

	float h = 1.0f - abs(uv.x) - abs(uv.y);
	if (h >= 0.0f)
	{
		xyz = vec3(uv.x, uv.y, h);
	}
	else
	{
		xyz.x = sign(uv.x) * (1.0f - abs(uv.y));
		xyz.y = sign(uv.y) * (1.0f - abs(uv.y));
		xyz.z = h;
	}

	return xyz;
}

vec2 MappingFromXYZToUV(vec3 xyz)
{
	vec2 uv;

	float d = abs(xyz.x) + abs(xyz.y) + abs(xyz.z);
	if (xyz.z <= 0.0f)
	{
		uv.x = xyz.x / d;
		uv.y = xyz.y / d;
	}
	else
	{
		uv.x = sign(xyz.x) * (1.0f - abs(xyz.y) / d);
		uv.y = sign(xyz.y) * (1.0f - abs(xyz.x) / d);
	}

	return uv;
}

void main()
{
	// Calculate vector from center to the position of this fragment, in impostor coordinates:
	vec2 fromCenterI = gs_out_textureCoordinates;
	float fromCenterDistanceI = length(fromCenterI);

	// Discard fragments with |(s, t)| > 1, in order to draw spheres:
	if (fromCenterDistanceI > 1.0f)
		discard;

	// Calculate new z value in world space:
	float z = gs_out_positionW.z - (1.0f - fromCenterDistanceI) * gs_out_radius;
	vec3 positionW = vec3(gs_out_positionW.x, gs_out_positionW.y, z);

	// Calculate the position in clip space and then perform the perspective divide on z to calculate the correct depth of the fragment:
	vec4 positionH = u_viewProjectionMatrix * vec4(positionW, 1.0f);
	gl_FragDepth = positionH.z / positionH.w;

	// Calculate normal in view space:
	vec3 normalV = vec3(fromCenterI.x, fromCenterI.y, 1.0f - fromCenterDistanceI);

	// Transform normal from view space to world space:
	vec3 normalW = (u_inverseViewMatrix * vec4(normalV, 0.0f)).xyz;
	normalW = normalize(normalW);

	// TODO
	Light lights[MAX_NUM_LIGHTS];
	lights[0].Strength = vec3(0.8f, 0.8f, 0.8f);
	lights[0].FalloffStart = 100.0f;
	lights[0].FalloffEnd = 200.0f;
	lights[0].Position = vec3(0.0f, 0.0f, -100.0f);

	// Create a material:
	Material material;
	material.DiffuseAlbedo = vec4(gs_out_color, 1.0f);
	material.FresnelR0 = u_materialFresnelR0;
	material.Shininess = u_materialShininess;

	// Compute lighting:
	vec4 lightIntensity = ComputeLighting(lights, material, positionW, normalW, u_eyePositionW);
	vec4 ambientIntensity = vec4(0.05f, 0.05f, 0.05f, 0.0f);

	// The final color is a sum of the light and ambient intensities:
	vec4 color = lightIntensity + ambientIntensity;
	color.a = 1.0f;

	fs_out_color = color;
}
