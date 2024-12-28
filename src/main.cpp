#include <iostream>
#include <raylib.h>
#include "tsettings.hpp"
#include "movement.hpp"
#include "map_gen.hpp"

int main() {
    std::cout << "hello world\n";
    float iTime = 0;
    InitWindow(1920, 1080, "Hello World");
    ToggleBorderlessWindowed();
    SetTargetFPS(100);
    DisableCursor();

    
    Model Tree = LoadModel("res/Tree.glb");
    Shader BasicLighting = LoadShader("res/shaders/baselight.vs", "res/shaders/baselight.fs");
    Tree.materials[0].shader = BasicLighting;
    Vector3 LightPos = (Vector3){0, 100, 0};

    int MatModelLoc = GetShaderLocation(BasicLighting, "matModel");
    int camPosLoc = GetShaderLocation(BasicLighting, "viewPos");
    int iTimeLoc = GetShaderLocation(BasicLighting, "iTime");
    int lightposLoc = GetShaderLocation(BasicLighting, "LightPos");
    int ambientLoc = GetShaderLocation(BasicLighting, "ambientColor");
    SetShaderValue(BasicLighting, lightposLoc, &LightPos, SHADER_UNIFORM_VEC3);

    Vector3 ambientColor = (Vector3){1, 1, 1};

    AOTMovement movement;
    JsonSettings settings;
    Camera3D camera;
    camera.position = (Vector3){ 0.0f, 3.0f, -3.0f };
    camera.target = (Vector3){ 0.0f, 3.0f, 0.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy= 60.0f;
    camera.projection = CAMERA_PERSPECTIVE; 

    SetShaderValue(BasicLighting, camPosLoc, &camera.position.x, SHADER_UNIFORM_VEC3);
    SetShaderValue(BasicLighting, ShaderLocationIndex(BasicLighting.locs[SHADER_LOC_MATRIX_MODEL]), &ambientColor, SHADER_UNIFORM_VEC3);
    SetShaderValue(BasicLighting, iTimeLoc, &iTime, SHADER_UNIFORM_FLOAT);
    SetShaderValue(BasicLighting, ambientLoc, &ambientColor, SHADER_UNIFORM_VEC3);
    //Generate list of trees
    
    int mapSize = 1500;
    int halfMapSize = mapSize/2;

    int treecount = 200;
    for (int i = 0; i<treecount; i++) movement.Mdevice.Forest.add_Tree(halfMapSize);
    movement.Mdevice.sendTrees();
    while (!WindowShouldClose()) {
        //Update Stuff
        iTime+=GetFrameTime();
        camera.target = movement.CamTargPos().toRayVec();
        movement.rotation = -(float)GetMouseX()/1000;
        movement.upAngle = (float)GetMouseY()/1000;
        LightPos = camera.position;
        SetShaderValue(BasicLighting, camPosLoc, &camera.position, SHADER_UNIFORM_VEC3);

        //Draw Stuff
        BeginDrawing();
        BeginMode3D(camera);
            ClearBackground(BLACK);
            
            logVec3(movement.getPos(), "Camera pos: ");
            logVec3(movement.Mdevice.Rrope.endpos, "endPos RRope");
            //DrawGrid(100, 3);
            BeginShaderMode(BasicLighting);
                DrawPlane(vec3(0).toRayVec(), (Vector2){(float)mapSize, (float)mapSize}, DARKBROWN);
                
            EndShaderMode();

            camera.position = movement.update(GetFrameTime());

            for (int i = 0; i<treecount; i++) DrawModel(Tree, movement.Mdevice.Forest._trees[i].toRayVec(), 1, BROWN);

        EndMode3D();
        //CrossHair point
        DrawCircle(GetScreenWidth()/2, GetScreenHeight()/2, 2, WHITE);
        DrawFPS(10, 10);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}
