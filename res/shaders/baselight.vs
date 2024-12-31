#version 330 core

in vec3 vertexPosition;
in vec3 vertexNormal;
in vec4 vertexColor;

out vec4 fragColor;
out vec3 fragPosition;
out vec3 fragNormal;

uniform float iTime;
uniform mat4 matModel;
uniform mat4 matView;
uniform mat4 matProjection;
uniform vec3 viewPos;

float scale = 3;

void main()
{
    vec3 vertPos = vertexPosition;
    float dist = length(viewPos-vertexPosition);
    
    fragPosition = vec3(matModel * vec4(vertPos, 1.0));

    //Wind effect
    float posEffect = .05;
    float offset = 1+sin(  iTime*.5+fragPosition.x*posEffect+fragPosition.z*posEffect );
    fragPosition += vec3(0, 0, offset);

    fragNormal = vertexNormal;
    fragColor = vertexColor;
    
    gl_Position =  matProjection * matView * vec4(fragPosition, 1.0);
}