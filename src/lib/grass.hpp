#pragma once
#include <raylib.h>

class grass {
    private:
        Shader GrassShellShader = LoadShader("grass.vs", "grass.fs");
        int MatModelLoc = GetShaderLocation(GrassShellShader, "matModel");
        int camPosLoc = GetShaderLocation(GrassShellShader, "viewPos");
        int iTimeLoc = GetShaderLocation(GrassShellShader, "iTime");
        int lightposLoc = GetShaderLocation(GrassShellShader, "LightPos");
        int ambientLoc = GetShaderLocation(GrassShellShader, "ambientColor");
        
    public:
        grass() {
            
        }

        void update(Camera3D cam) {
            SetShaderValue(GrassShellShader, camPosLoc, &cam.position.x, SHADER_UNIFORM_VEC3);
        }

};