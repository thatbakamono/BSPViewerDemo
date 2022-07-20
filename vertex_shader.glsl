#version 330 core

layout (location = 0) in vec3 PositionAttribute;

void main() {
    gl_Position = vec4(PositionAttribute, 1.0);
}