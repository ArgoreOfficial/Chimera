#version 410 core

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

out vec4 color;

in DATA
{
   vec4 color;
   mat4 proj;
} data_in[];

void main() {
    vec4 p = gl_in[0].gl_Position;
    vec4 c = data_in[0].color;

    float w = 0.02f;

    gl_Position = data_in[0].proj * ( p + vec4( -w, -w, 0.0f, 0.0f ) ); 
    color = c;
    EmitVertex();
    
    gl_Position = data_in[0].proj * ( p + vec4( -w, w, 0.0f, 0.0f ) ); 
    color = c;
    EmitVertex();

    gl_Position = data_in[0].proj * ( p + vec4( w, -w, 0.0f, 0.0f ) ); 
    color = c;
    EmitVertex();

    gl_Position = data_in[0].proj * ( p + vec4( w, w, 0.0f, 0.0f ) ); 
    color = c;
    EmitVertex();

    EndPrimitive();
}  