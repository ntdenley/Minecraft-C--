#version 330 core
in vec2 v_texCoord;
in float v_light;

out vec4 f_color;

uniform sampler2D tex1;

void main( void ) 
{
    f_color = texture(tex1, v_texCoord) * v_light;
}