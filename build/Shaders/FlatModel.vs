#version 450 core
layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 Color;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat4 transform;

out vec3 outColor;

void main(void){
	
	gl_Position = projection * view * transform * model * vec4(Position, 1.0);
	
	outColor = Color;
	
}