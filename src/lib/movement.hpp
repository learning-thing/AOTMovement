#pragma once
#include "vec3.hpp"
#include <raylib.h>
#define TDEBUG true
#include "map_gen.hpp"

void logVec3(vec3 vecToLog, std::string label) {
    if (TDEBUG)
    std::clog << label << ": X:"<< vecToLog.x() <<  " Y:" << vecToLog.y() << " Z:" << vecToLog.z() << "\n";
}

class ManueverRope {
    public:
        vec3 startpos = vec3(0);
        vec3 endpos = vec3(0);
        TreeManager _Forest;
        bool grabbing = false;
        void seek(vec3 start, vec3 direction) {
            logVec3(start, "Received position: ");
            std::clog << "seeking\n";
            vec3 s = startpos;
            vec3 dir = direction-startpos;
            dir.make_unit_vector();

            for (int i = 0; i< 500; i++) {
                s+= dir*2;
                //Add Tree collision code here later
                if (_Forest.collides(s)) {
                    std::clog << "Hit something\n";
                    endpos = s;
                    logVec3(endpos, "Hit at position: ");
                    grabbing=true;
                    break;
                }


                if (s.y()<=0) {
                    std::clog << "Hit something\n";
                    grabbing=true;
                    endpos = s*vec3(1, 0, 1);
                    logVec3(endpos, "Hit at position: ");
                    break;
                }
            }
        }
        void reset() {
            
            grabbing=false;
        }

        void draw() {
            if (!grabbing) return;
            DrawLine3D(endpos.toRayVec(), (startpos-vec3(0, 3, 0)).toRayVec(), RED);//Main Rope videualisation
            DrawLine3D(endpos.toRayVec(), (startpos-vec3(0, 3, 0)).toRayVec(), RED);//Main Rope videualisation

            DrawCubeV(endpos.toRayVec(),   vec3(1).toRayVec(), GREEN);
            //DrawCubeV(startpos.toRayVec(), vec3(1).toRayVec(), BLUE);
            
            logVec3(startpos, "\n\nstartpos: ");
            //logVec3(endpos, "endpos: ");
        }

        vec3 getPullVec() {
            vec3 dir = endpos-startpos;
            dir.make_unit_vector();
            if (grabbing)
            return dir;
            else return vec3(0);
        }
};

class ManueverDevice {
    public:
        ManueverRope Lrope;
        ManueverRope Rrope;
        TreeManager Forest;
        void sendTrees()  {
            Lrope._Forest = Forest;
            Rrope._Forest = Forest;
        }

        void update(vec3 pos, vec3 lookdir) {
            Lrope.startpos = pos;
            Rrope.startpos = pos;
            Lrope.draw();
            Rrope.draw();
            
            //DrawLine3D(Lrope.endpos.toRayVec(), vec3(0).toRayVec(), BLUE);

            if (IsKeyPressed(KEY_Q)) Lrope.seek(pos, lookdir);
            if (IsKeyPressed(KEY_E)) Rrope.seek(pos, lookdir);

            if (IsKeyReleased(KEY_Q)) Lrope.reset();
            if (IsKeyReleased(KEY_E)) Rrope.reset();
            
        }

        vec3 GetPullVector() {
            vec3 ret = Lrope.getPullVec()*.5 + Rrope.getPullVec()*.5;
            ret.make_unit_vector();
            return ret;
        }
};



class AOTMovement {
    private:
        vec3 position = vec3(0, 3, -5);
        vec3 speed = vec3(0);
        vec3 acceleration = vec3(0);
        vec3 gravity = vec3(0, 5, 0);
        vec3 strafevec = vec3(0);
        float floorheight = 3;
        float frameTime = 0;
        bool debug = false;

        void DBGLOG(std::string msg) {
            if (debug) {
                std::clog << msg << "\n";
            }
        }
        void DBGLOG(float msg) {
            if (debug) {
                std::clog << msg << "\n";
            }
        }

        void input() {
            if (IsKeyDown(forwardKey))   MoveForward();
            if (IsKeyDown(backwadsKey))  MoveBackward();
            //if (IsKeyDown(turnRightKey)) turnRight();
            //if (IsKeyDown(turnLeftKey)) turnLeft();
            if (IsKeyDown(MoveLeftKey)) MoveLeft();
            if (IsKeyDown(MoveRightKey)) MoveRight();
            if (IsKeyPressed(JumpKey)) Jump();
        }
        void MoveForward() {
            acceleration+=dirvec*accelSpeed*vec3(1,0,1);
        }

        void MoveBackward() {
            acceleration-=dirvec*accelSpeed*vec3(1,0,1);
        }

        void MoveRight() {
            acceleration-=strafevec*accelSpeed;
        }

        void MoveLeft() {
            acceleration+=strafevec*accelSpeed;
        }

        void Jump() {
            if (abs(position.y()-floorheight)<.01f) {
                acceleration+=vec3(0, 500, 0);
            }
        }

    public:
        vec3 dirvec = vec3(0);
        float upAngle = 0;
        float rotation = 0;
        int forwardKey = KEY_W;
        int backwadsKey = KEY_S;
        int turnRightKey = KEY_RIGHT;
        int turnLeftKey = KEY_LEFT;
        int MoveLeftKey = KEY_A;
        int MoveRightKey = KEY_D;
        int JumpKey = KEY_SPACE;
        float accelSpeed = 27;
        float deceleration = 1;
        float height = 0;
        ManueverDevice Mdevice;


        Vector3 update(float frametime) {
            frameTime = frametime;
            input();
            dirvec = vec3(sin(rotation), sin(upAngle), cos(rotation));
            strafevec = vec3(cos(-rotation), 0, sin(-rotation));
            
            gravity = vec3(0, position.y()-floorheight, 0);
            acceleration-=gravity;
            if (position.y()<floorheight || abs(floorheight-position.y())<.01f) {
                DBGLOG("Resetting to groundHeight");
                position.e[1]=floorheight;
            }
            DBGLOG( position.y() );
            if (IsKeyDown(KEY_SPACE))
                if (Mdevice.Lrope.grabbing)
                    acceleration+=Mdevice.GetPullVector()*100;
            logVec3(Mdevice.GetPullVector(), "Direction we pull to: ");
            
            speed+=acceleration*frametime;
            position+=speed*frametime;
            speed+=acceleration*frametime;
            
            if (position.y()==floorheight)
                speed-=5*speed*frameTime;
            else speed-=speed*frameTime;

            //position += speed * frametime + 0.5 * acceleration * frametime * frametime;
            speed += acceleration * frametime;

            acceleration = 0;
            logVec3(position, "Actual movement position: ");
            Mdevice.update(position, CamTargPos());
            //std::clog << position.x() << " : " << position.z() << "\n";
            return position.toRayVec();
        }

        vec3 CamTargPos() {
            return position+dirvec*2;
        }

        vec3 CamTargOffset(float scale) {
            return dirvec*scale;
        }

        vec3 CamThirdPersonTargPos() {
            return position+dirvec*5;
        }

        vec3 getPos() {
            return position+vec3(0, height, 0);
        }
};

