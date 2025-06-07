#version 330 core


out vec4 FragColor;

in vec3 pointColor;

uniform bool u_isEdge;
uniform vec4 u_edgeColor;
uniform bool u_isDarkPiece;
uniform float u_darkeningFactor;

void main()
{
    if (u_isEdge) {
        FragColor = u_edgeColor; // Opaque black for edges, for example
    } else {
        vec3 finalColor = pointColor;
        if (u_isDarkPiece) {
            finalColor *= u_darkeningFactor;
        }
        FragColor = vec4(finalColor, 1.0f);
    }
}