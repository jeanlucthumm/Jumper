#version 330 core

layout (location = 0) in vec3 position;

uniform mat4 projection;
uniform mat4 modelview;

void main() {
	gl_PointSize = 15.0;
	gl_Position = projection * modelview * vec4(position, 1.0);
}
