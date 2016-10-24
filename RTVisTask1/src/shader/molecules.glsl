-- Vertex

#extension GL_ARB_explicit_attrib_location : enable

// Uniforms:
uniform mat4 u_viewProjectionMatrix;

// Input:
in vec3 vs_in_positionW;
in vec3 vs_in_color;
in float vs_in_radius;

// Output:
out vec3 vs_out_color;
out float vs_out_radius;

void main()
{
	gl_Position = u_viewProjectionMatrix * vec4(vs_in_positionW, 1.0f);
	vs_out_color = vs_in_color;
}

/*-- Geometry

variables

void main()
{	
} */ 

--Fragment

// Input:
in vec3 vs_out_color;
in float vs_out_radius;

// Output:
out vec4 fs_out_color;

void main()
{
	fs_out_color = vec4(vs_out_color.x, vs_out_color.y, vs_out_radius, 1.0f);
}
