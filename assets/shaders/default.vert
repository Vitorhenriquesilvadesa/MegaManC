#version 330 core

layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aTexCoord;

out vec2 Position;
out vec2 TexCoord;

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;
uniform int frameCount;
uniform int currentFrame;
uniform int isTiled;
uniform vec2 scale;

void main(void) {
    vec4 worldPosition = projection * view * model * vec4(aPos.xy, 0.0, 1.0);
    Position = vec2(worldPosition.xy);

    float frameWidth = 1.0 / float(frameCount);
    float offset = frameWidth * float(currentFrame);

    if(isTiled == 0) {
        TexCoord = vec2(aTexCoord.x * frameWidth + offset, aTexCoord.y);
    } else {
        TexCoord = aTexCoord * scale;
    }

    gl_Position = worldPosition;
}