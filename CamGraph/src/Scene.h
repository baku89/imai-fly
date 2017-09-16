#pragma once

#include "ofMain.h"
#include "ofBaku.h"
#include "ofxAdvancedXmlSettings.h"

class FrameData {
public:
    
    bool			empty = true;
    ofVec3f			pos;
    ofVec3f			rot;
    float           speed = 0;
    string			hash = "";
    string			filePath = "";
    
    float yaw() {
        return rot.y;
    }
    
    float pitch() {
        return rot.x;
    }
    
    float roll() {
        return rot.z;
    }
};

class Scene {
public:
    
    string& getName() {
        return name;
    }
    
    void setDuration(size_t duration) {
        while (data.size() < duration) {
            data.push_back(new FrameData());
        }
    }
    
    size_t getDuration() {
        return data.size();
    }
    
    FrameData* get(int frame) {
        return data[frame];
    }
    
    FrameData* getPrev(int frame) {
        
        FrameData *prevFd = data[frame];
        
        for (int i = frame - 1; i >= 0; i--) {
            if (!data[i]->empty) {
                prevFd = data[i];
                break;
            }
        }
        
        return prevFd;
    }
    
    void load(const string &_name) {
        
        name = _name;
        dirPath = "";
        
        ofxAdvancedXmlSettings sceneSettings;
        
        data.clear();
        
        bool exists = sceneSettings.load("scenes/scene_" + name + ".xml");
        
        if (exists) {
            
            sceneSettings.pushTag("data");
            {
                int numFrames = sceneSettings.getNumTags("frameData");
                this->setDuration(numFrames);
                
                for (int i = 0; i < numFrames; i++) {
                    
                    sceneSettings.pushTag("frameData", i);
                    
                    FrameData *fd = data[i];
                    fd->empty	= sceneSettings.getValue("empty",	fd->empty);
                    fd->pos		= sceneSettings.getValue("pos",		fd->pos);
                    fd->rot		= sceneSettings.getValue("rot",		fd->rot);
                    fd->speed   = sceneSettings.getValue("speed",   fd->speed);
                    fd->hash	= sceneSettings.getValue("hash",	fd->hash);
                    
                    sceneSettings.popTag();
                }
            }
            sceneSettings.popTag();
            
            dirPath = sceneSettings.getValue("dirPath", "");
        }
    }
    
    void save(bool saveAsBackup = false) {
        
        // save Scene
        ofxAdvancedXmlSettings sceneSettings;
        
        sceneSettings.addPushTag("data");
        {
            for (auto& fd : data) {
                
                int i = sceneSettings.addTag("frameData");
                
                sceneSettings.pushTag("frameData", i);
                {
                    sceneSettings.setValue("empty", fd->empty);
                    sceneSettings.setValue("pos",	fd->pos);
                    sceneSettings.setValue("rot",	fd->rot);
                    sceneSettings.setValue("speed",	fd->speed);
                    sceneSettings.setValue("hash",	fd->hash);
                }
                sceneSettings.popTag();
            }
        }
        sceneSettings.popTag();
        
        sceneSettings.setValue("dirPath", dirPath);
        
        string path;
        
        if (saveAsBackup) {
            path = "scenes/backups/scenes_" + name + "_" + ofGetTimestampString() + ".xml";
        } else {
            path = "scenes/scene_" + name + ".xml";
        }
        
        sceneSettings.save(path);
    }
    
    void conform() {
        
        ofDirectory sceneDir(dirPath);
        
        if (!sceneDir.exists()) {
            return;
        }
        
        // create backup
        this->save(true);
        
        // TODO: Add RAW extensions
        //sceneDir.allowExt("jpg");
        sceneDir.allowExt("cr2");
        
        sceneDir.listDir();
        vector<ofFile> files = sceneDir.getFiles();
        
        // make a dict of hash with existing scene data
        map<string, FrameData*> frameMap;
        
        for (auto& fd : data) {
            if (!fd->empty) {
                frameMap[fd->hash] = fd;
            }
        }
        
        // assign new frame
        data.clear();
        
        for (auto& file : files) {
            
            string baseName = file.getBaseName();
            string filePath = file.getAbsolutePath();
            
            string seqStr	= baseName.substr(baseName.length() - 4);
            int frame		= ofToInt(seqStr);
            
            string hash		= ofGetFileHash(filePath);
            
            this->setDuration(frame + 1);
            
            if (frameMap[hash]) {
                data[frame] = frameMap[hash];
            } else {
                ofLogNotice() << "sortCurrentScene(): Invalid hash found.";
            }
        }
        
        // calc speed
        ofVec3f prevPos = data[0]->pos;
        
        for (auto& fd : data) {
            
            fd->speed = (fd->pos - prevPos).length();
            prevPos = fd->pos;
        }
    }
    
    void setDirPath(const string &_dirPath) {
        dirPath = _dirPath;
    }
    
private:
    
    string name, dirPath;
       vector<FrameData*> data;
    
};
