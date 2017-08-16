#pragma once

#include "ofMain.h"
#include "ofBaku.h"
#include "ofxAdvancedXmlSettings.h"

class FrameData {
public:
    
    bool			empty = true;
    ofVec3f			pos;
    ofVec3f			rot;
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
        ofLogNotice() << duration;
        while (data.size() < duration) {
            data.push_back(new FrameData());
        }
    }
    
    size_t getDuration() {
        return data.size();
    }
    
    FrameData* get(int frame) {
        return data[frame - 1];
    }
    
    void load(const string &_name) {
        
        name = _name;
        dirPath = "";
        
        ofxAdvancedXmlSettings sceneSettings;
        
        bool exists = sceneSettings.load("scene_" + name + ".xml");
        
        data.clear();
        
        
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
                    fd->hash	= sceneSettings.getValue("hash",	fd->hash);
                    data.push_back(fd);
                    
                    sceneSettings.popTag();
                }
            }
            sceneSettings.popTag();
            
            dirPath = sceneSettings.getValue("dirPath", "");
        }
    }
    
    void save() {
        
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
                    sceneSettings.setValue("hash",	fd->hash);
                }
                sceneSettings.popTag();
            }
        }
        sceneSettings.popTag();
        
        sceneSettings.setValue("dirPath", dirPath);
        
        sceneSettings.save("scene_" + name + ".xml");
    }
    
    void confirm() {
        
        ofDirectory sceneDir(dirPath);
        
        if (!sceneDir.exists()) {
            return;
        }
        
        // TODO: Add RAW extensions
        sceneDir.allowExt("png");
        sceneDir.allowExt("jpg");
        sceneDir.allowExt("tiff");
        
        
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
            int fnum		= ofToInt(seqStr);
            
            string hash		= ofGetFileHash(filePath);
            
            while (data.size() < fnum - 1) {
                data.push_back(new FrameData());
            }
            
            if (frameMap[hash]) {
                data.push_back(frameMap[hash]);
            } else {
                ofLogNotice() << "sortCurrentScene(): Invalid hash found.";
            }
        }
    }
    
    void setDirPath(const string &_dirPath) {
        dirPath = _dirPath;
    }
    
private:
    
    string name, dirPath;
       vector<FrameData*> data;
    
};
