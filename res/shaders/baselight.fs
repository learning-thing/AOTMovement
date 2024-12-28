#version 330

out vec4 finalColor;

in vec4 fragColor;
in vec3 fragPosition;
in vec3 fragNormal;

uniform vec3 LightPos;
uniform vec3 viewPos;
uniform vec3 ambientColor;
//uniform vec3 cameraSource;

vec3 lightPosition = LightPos;
float ambientStrength = 0.09;

void main()
{
    vec4 col = fragColor;
    if (fragPosition.y>50) {//Lazy ass-dumbass way to give the trees color lmao
        col = vec4(0.051, 0.4471, 0.0, 1.0);
    } else {
        col = vec4(0.4471, 0.1882, 0.0157, 1.0);
    }

    // ambient
    vec3 ambient = ambientStrength * ambientColor;
    float dist =  length(viewPos-fragPosition);
    // diffuse 
    vec3 norm = normalize(fragNormal);//Frag Normal
    vec3 lightDir = normalize(lightPosition - fragPosition);//normalised Light direction vector
    float diff = max(dot(norm, lightDir), 0.0);//Raw diffused value
    vec3 diffuse = diff * ambientColor;//Final Diffused Value

    //Shadow
    vec3 s = fragPosition;
    //float ssize = length(lightDir);
    float LightDist = length(LightPos-fragPosition);
    if (LightDist>dist) {
        for(int i = 0; i < 100; i++) {
            //s+=lightDir/3;//Our Light Checker
            s+=lightDir*dist;//Our Light Checker
            
            if (length(s-viewPos)<1) {
                vec3 shadowDiffuse = diffuse;
                diffuse = mix(ambient, shadowDiffuse, dist/LightDist);//Draw The shadow
                break;
            }
        }
    }
    
    vec3 result = (ambient + diffuse) * col.rgb; 
    finalColor = vec4(result, col.a);
} 