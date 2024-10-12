#version 330 core
in vec3 v_color;
in vec2 v_texCoord;

out vec4 f_color;

uniform sampler2D tex1;
uniform sampler2D tex2;

void main( void ) 
{
    f_color = mix(texture(tex1, v_texCoord), texture(tex2, v_texCoord), 0.2);
}