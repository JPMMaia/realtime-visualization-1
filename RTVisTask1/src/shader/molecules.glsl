//this file contains a basic shader structure you can use. It will automatically be loaded
//follow comments in the cpp file (glwidget) for more information 
//The "--InsterWord" indicate where one shader begins/ends
// for example the vertex shader begins at "--Vertex" and ends at "--Geometry"

-- Vertex

#extension GL_ARB_explicit_attrib_location : enable

struct PassConstants
{
	mat4 ViewProjectionMatrix;
};

uniform PassConstants g_pass;

in vec3 vs_in_positionW;
in vec3 vs_in_color;

out vec3 vs_out_color;

void main()
{
	gl_Position = g_pass.ViewProjectionMatrix * vec4(vs_in_positionW, 1.0f);
	vs_out_color = vs_in_color;
}

/*-- Geometry

variables

void main()
{	
} */ 

--Fragment

struct VertexOutput
{
	vec3 Color;
};

in vec3 vs_out_color;
out vec4 fs_out_color;

void main()
{
	fs_out_color = vec4(vs_out_color, 1.0f);
}
