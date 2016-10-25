-- Vertex

#extension GL_ARB_explicit_attrib_location : enable

#include "LightingUtils.glsli"

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
uniform mat4 u_projectionMatrix;
uniform mat4 u_viewProjectionMatrix;
uniform vec3 u_eyePositionW;

// Input:
in vec3 vs_out_positionW[];
in vec3 vs_out_color[];
in float vs_out_radius[];

// Output:
out vec3 gs_out_positionV;
flat out vec3 gs_out_color;
flat out float gs_out_radiusV;
flat out vec3 gs_out_centerV;

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

	// Emit vertices:
	for (int i = 0; i < 4; ++i)
	{
		gl_Position = u_projectionMatrix * verticesV[i];
		gs_out_positionV = verticesV[i].xyz;
		gs_out_color = vs_out_color[0];
		gs_out_radiusV = radiusV;
		gs_out_centerV = centerV;
		EmitVertex();
	}
	EndPrimitive();
} 

--Fragment

// Input:
in vec3 gs_out_positionV;
flat in vec3 gs_out_color;
flat in float gs_out_radiusV;
flat in vec3 gs_out_centerV;

// Output:
out vec4 fs_out_color;

void main()
{
	vec3 fromCenterVector = gs_out_positionV - gs_out_centerV;
	if (dot(fromCenterVector, fromCenterVector) > gs_out_radiusV * gs_out_radiusV)
		discard;

	fs_out_color = vec4(gs_out_color, 1.0f);
}
