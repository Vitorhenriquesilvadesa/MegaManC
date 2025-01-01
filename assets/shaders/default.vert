#version 330 core

layout(location = 0) in vec2 aPos;

out vec2 Position;

void main(void) {
    Position = aPos;
    gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);
}