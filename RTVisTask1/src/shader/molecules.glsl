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
uniform mat4 u_viewProjectionMatrix;
uniform vec3 u_eyePositionW;

// Input:
in vec3 vs_out_positionW[];
in vec3 vs_out_color[];
in float vs_out_radius[];

// Output:
out vec3 gs_out_positionW;
out vec3 gs_out_color;
out float gs_out_radius;

void main()
{
	vec3 centerW = vs_out_positionW[0];
	float radius = vs_out_radius[0];
	
	// Calculate the local coordinate system:
	vec3 up = vec3(0.0f, 1.0f, 0.0f);
	vec3 forward = normalize(u_eyePositionW - centerW);
	vec3 right = cross(up, forward);

	// Compute the position of the vertices of the billboard:
	vec4 vertices[4];
	vertices[0] = vec4(centerW - up * radius - right * radius, 1.0f);
	vertices[1] = vec4(centerW - up * radius + right * radius, 1.0f);
	vertices[2] = vec4(centerW + up * radius - right * radius, 1.0f);
	vertices[3] = vec4(centerW + up * radius + right * radius, 1.0f);

	// Emit vertices:
	for (int i = 0; i < 4; ++i)
	{
		gl_Position = u_viewProjectionMatrix * vertices[i];
		gs_out_positionW = vertices[i].xyz;
		gs_out_color = vs_out_color[0];
		gs_out_radius = vs_out_radius[0];
		EmitVertex();
	}
	EndPrimitive();
} 

--Fragment

// Input:
in vec3 gs_out_positionW;
in vec3 gs_out_color;
in float gs_out_radius;

// Output:
out vec4 fs_out_color;

void main()
{
	fs_out_color = vec4(gs_out_color, 1.0f);
}
