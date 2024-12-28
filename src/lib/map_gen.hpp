#pragma once
#include "tsettings.hpp"
#include <vector>

class TreeManager {
    private:
        int treeCount = 0;
        uint treewidth = 10;
    public:
        std::vector<vec3> _trees;

        void add_Tree(float halfMapSize) {
            vec3 TreePos = vec3((float)GetRandomValue(-halfMapSize, halfMapSize), 0, (float)GetRandomValue(-halfMapSize, halfMapSize));
            _trees.push_back(TreePos);
            treeCount++;
        }

        void AdjustSpawnPos(vec3 targ) {
            for (int i = 0; i < treeCount; i++)  {

            }
        }

        bool collides(vec3 ppos)  {
            for (int i = 0; i<treeCount; i++)
                if ( (ppos*vec3(1, 0, 1) - _trees[i]*vec3(1, 0, 1)).length() < 5 && ppos.y()<100)
                    return true;
            return false;
        }

};