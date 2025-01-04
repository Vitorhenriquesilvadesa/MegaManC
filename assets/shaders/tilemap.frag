#version 330 core

in vec2 Position;
in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D albedo;

void main(void) {
    vec4 color = texture(albedo, TexCoord);

    if(color.a == 0.0) {
        discard;
    }
    
    FragColor = color;
}