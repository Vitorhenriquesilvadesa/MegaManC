#version 330 core

layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aTexCoord;

out vec2 Position;
out vec2 TexCoord;

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;
uniform int atlasRows;   
uniform int atlasColumns;
uniform int id;          

void main(void) {
    vec4 worldPosition = projection * view * model * vec4(aPos.xy, 0.0, 1.0);
    Position = vec2(worldPosition.xy);

    int row = id / atlasColumns;    
    int column = id % atlasColumns; 

    float tileWidth = 1.0 / float(atlasColumns);
    float tileHeight = 1.0 / float(atlasRows);

    float offsetX = float(column) * tileWidth;
    float offsetY = float(row) * tileHeight;

    TexCoord = vec2(
        aTexCoord.x * tileWidth + offsetX,
        aTexCoord.y * tileHeight + offsetY
    );

    gl_Position = worldPosition;
}
