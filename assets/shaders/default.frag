#version 330 core

in vec2 Position;

out vec4 FragColor;

void main(void) {
    FragColor = vec4(Position.x, Position.y, 1.0, 1.0);
}