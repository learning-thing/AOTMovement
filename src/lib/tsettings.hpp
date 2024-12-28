#pragma once
#include <fstream>
#include <string>
#include <json/json.h>
#include <iostream>
#include "vec3.hpp"

class JsonSettings {
    private:
        std::string fileName="settings.json";
        Json::Value settings;
        bool loaded = false;
    public:
        JsonSettings() {
            Load();
        }

        JsonSettings(std::string _fileName) : fileName(_fileName) {
            Load();
        }

        bool Load() {
            /*
            Loads the settings from the settings file
            returns true on success, false on failure
            */
           std::ifstream jsonFile(fileName, std::ifstream::binary);
           if (!jsonFile.is_open()) return false;
           jsonFile >> settings;
           //std::clog << settings << "\n";
           jsonFile.close();
           loaded = true;
           return true;
        }

        bool Save() {
            /*
            Loads the settings from the settings file.
            returns true on success, false on failure
            */
           if (!loaded) std::cerr << "You haven't even loaded your json file yet!!\n";
           std::ofstream jsonFile(fileName, std::ofstream::binary);
           jsonFile<<settings;
           return true;
        }

        int GetIntValueFor(std::string pars) {
           if (!loaded) std::cerr << "You haven't even loaded your json file yet!!\n";
            int ret = 0;
            ret = settings.get(pars.c_str(), 6).asInt();
            return ret;
        }

        int GetFloatValueFor(std::string pars) {
           if (!loaded) std::cerr << "You haven't even loaded your json file yet!!\n";
            float ret = 0;
            ret = settings.get(pars.c_str(), 6).asFloat();
            return ret;
        }

        bool GetBoolValueFor(std::string pars) {
           if (!loaded) std::cerr << "You haven't even loaded your json file yet!!\n";
            bool ret = false;
            ret = settings.get(pars.c_str(), false).asBool();
            return ret;
        }

        vec3 GetVec3ValueFor(std::string pars) {
           if (!loaded) std::cerr << "You haven't even loaded your json file yet!!\n";
            const Json::Value a = settings[pars];
            if (a.isNull()) {
                std::cerr << "Key not found: " << pars << "\n";
                return vec3(0);
            }
            
            if (!a.isArray() || a.size() != 3) {
                std::cerr << "Expected an array of size 3 for key: " << pars << "\n";
                return vec3(0);
            }

            return vec3(a[0].asFloat(), a[1].asFloat(), a[2].asFloat());
        }

        std::string GetStrValueFor(std::string pars) {
           if (!loaded) std::cerr << "You haven't even loaded your json file yet!!\n";
            std::string ret;
            ret = settings.get(pars.c_str(), "").asString();
            return ret;
        }

        bool SetValueFor(std::string pars, int Value) {
           if (!loaded) std::cerr << "You haven't even loaded your json file yet!!\n";
            settings[pars] = Value;
            return true;
        }
        bool SetValueFor(std::string pars, std::string Value) {
            if (!loaded) std::cerr << "You haven't even loaded your json file yet!!\n";
            settings[pars] = Value;
            return true;
        }
        bool SetValueFor(std::string pars, bool Value) {
            if (!loaded) std::cerr << "You haven't even loaded your json file yet!!\n";
            settings[pars] = Value;
            return true;
        }
        bool SetValueFor(std::string pars, float Value) {
            if (!loaded) std::cerr << "You haven't even loaded your json file yet!!\n";
            settings[pars] = Value;
            return true;
        }
        bool SetValueFor(std::string pars, vec3 Value) {
            if (!loaded) std::cerr << "You haven't even loaded your json file yet!!\n";
            settings[pars][0] = Value.x();
            settings[pars][1] = Value.y();
            settings[pars][2] = Value.z();
            return true;
        }
};