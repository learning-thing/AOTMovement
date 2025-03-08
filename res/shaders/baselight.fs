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
float ambientStrength = 0.2;

float scale = 3;

void main()
{
    vec4 col = fragColor;
    

    // ambient
    //vec3 ambient = ambientStrength * ambientColor;
    vec3 ambient = ambientStrength * vec3(0.2196, 0.2196, 0.2196);
    float dist =  length(viewPos-fragPosition);
    // diffuse 
    vec3 norm = normalize(fragNormal);//Frag Normal
    vec3 lightDir = normalize(lightPosition - fragPosition);//normalised Light direction vector
    float diff = max(dot(norm, lightDir), 0.0);//Raw diffused value angle off lightdirection and normal
    vec3 diffuse = mix(diff * ambientColor*2, col.xyz, .5);//Final Diffused Value

    if (fragPosition.y>78*scale) {//Lazy ass-dumbass way to give the trees color lmao
        col = vec4(0.0353, 0.1686, 0.0157, 1.0);
    } else {
        col = vec4(0.1451, 0.0745, 0.0275, 1.0);
    }
    //col.xyz -= vec3(dist*.0002);

    //Shadow
    vec3 s = fragPosition;
    //float ssize = length(lightDir);
    float LightDist = length(LightPos-fragPosition);
    if (LightDist>dist) {
        for(int i = 0; i < 100; i++) {//Shadow marching
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