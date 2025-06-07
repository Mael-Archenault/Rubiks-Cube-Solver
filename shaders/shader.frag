#version 330 core


out vec4 FragColor;

in vec3 pointColor;

uniform int u_faceIndex;
uniform bool u_faceVisible[6];


void main()
{
    if (u_faceIndex >= 0 && u_faceIndex < 6 && !u_faceVisible[u_faceIndex])
    {
        FragColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
        return;
    }
    FragColor = vec4(pointColor, 1.0f);
    
}