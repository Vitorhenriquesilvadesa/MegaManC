#version 330 core

layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aTexCoord;

out vec2 Position;
out vec2 TexCoord;

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;

void main(void) {
    vec4 worldPosition = projection * view * model * vec4(aPos.xy, 0.0, 1.0);
    Position = vec2(worldPosition.xy);
    TexCoord = aTexCoord;
    gl_Position = worldPosition;
}