#pragma once

//#include <math.h>

#include "ofxAdvancedXmlSettings.h"
#include "Graph.h"

#define PI 3.14159265


class RadialDolly {
public:
    
    void setup() {
        
        startAngle = 0;
        endAngle = 120.0f / 108.0f * PI;
        
        graph.setColor(ofColor(255, 128, 128));
        graph.setMarginY(0, 0);
        
        
        // sceneData
        //SceneData f0, f1, f2, f3, f4;
        SceneData f2;
        
        f2.startFrame = 2522;
        f2.endFrame = 2556;
        
        for (int f = f2.startFrame; f < f2.endFrame; f++) {
            f2.kf.push_back((float)(f - f2.startFrame) /
                            (float)(f2.endFrame - f2.startFrame));
        }
        
        data["FLY_4F2_01"] = f2;
        
    }
    
    void changeScene(const string &newSceneName) {
        currentSceneName = newSceneName;
    }
    
    void loadSettings(ofxAdvancedXmlSettings &s) {
        
        s.pushTag("radialDolly");
        
        isShowing   = s.getValue("isShowing", isShowing);
        radius      = s.getValue("radius", radius);
        pivot       = s.getValue("pivot", pivot);
        startAngle  = s.getValue("startAngle", startAngle);
        endAngle    = s.getValue("endAngle", endAngle);
        
        
        s.popTag();
        
        // accessing global settings
        currentFrame     = s.getValue("currentFrame", 0);
        currentSceneName = s.getValue("currentSceneName", "");
    }
    
    void saveSettings(ofxAdvancedXmlSettings &s) {
        
        s.addPushTag("radialDolly");
        
        s.setValue("isShowing", isShowing);
        s.setValue("radius", radius);
        s.setValue("pivot", pivot);
        s.setValue("startAngle", startAngle);
        s.setValue("endAngle", endAngle);
        
        
        s.popTag();
    }
    
    void update(ofVec3f _pos, int _currentFrame) {
        pos = _pos;
        currentFrame = _currentFrame;
    }
    
    void addGraphValue(const int f, ofVec3f &_pos, const bool calcRange) {
        
        static float x, y, a;
        
        x = _pos.z - pivot.z;
        y = _pos.x - pivot.x;
        
        a = fmod(atan2(y, x) + PI * 2, PI * 2);//atan2(y, x);
        
        graph.addValue(f, a, calcRange);
    }
    
    void draw() {
        
        if (!isShowing) {
            return;
        }
        
        ofPushStyle();
        
        // draw axis
        ofSetLineWidth(3);
        ofSetColor(255);
        ofDrawLine(pivot.x, -2, pivot.z, pivot.x, 2, pivot.z);
        
        // draw radius
        ofSetLineWidth(2);
        ofSetColor(255, 0, 0);
        ofDrawLine(pos.x, pos.y, pos.z, pivot.x, pos.y, pivot.z);
        
        // draw ring
        ofNoFill();
        ofSetLineWidth(1);
        ofSetColor(180);
        ofPushMatrix();
        ofRotateX(90);
        ofSetCircleResolution(96);
        ofDrawCircle(pivot.x, pivot.z, -pos.y, radius);
        ofPopMatrix();
        ofFill();
        
        // draw start and end point
        ofSetLineWidth(2);
        ofSetColor(192, 192, 0);
        ofDrawLine(pivot.x, pos.y, pivot.z,
                   pivot.x + sin(startAngle) * radius,
                   pos.y,
                   pivot.z + cos(startAngle) * radius);
        
        ofSetColor(0, 192, 192);
        ofDrawLine(pivot.x, pos.y, pivot.z,
                   pivot.x + sin(endAngle) * radius,
                   pos.y,
                   pivot.z + cos(endAngle) * radius);
        
        
        ofPopStyle();
    }
    
    void drawGraph(ofRectangle &rect, const int xmin, const int xmax) {
        
        if (isShowing) {
            graph.setRangeX(xmin, xmax);
            graph.setRangeY(startAngle, endAngle);
            graph.draw(rect);
        }
    }
    
    void drawImGui() {
        
        if (ImGui::CollapsingHeader("Radial Dolly")) {
            
            ImGui::Checkbox("Show in Viewer", &isShowing);
            
            ImGui::SliderFloat("Radius", &radius, 0.1, 5.0);
            
            if (ImGui::Button("Set Pivot")) {

                pivot = pos;
                pivot.y = 0;
            }
            
            ImGui::SameLine();
            
            if (ImGui::Button("Set Start")) {
                
                startAngle = calcCurrentAngle();
                
                ofLogNotice() << "startAngle: " << startAngle / PI * 180;
            }
            
            ImGui::SameLine();
            
            if (ImGui::Button("Set End")) {
                
                endAngle = calcCurrentAngle();
                
                ofLogNotice() << "endAngle: " << endAngle / PI * 180;
            }
            
        }
    }
    
    
    Graph       graph;
    bool        isShowing = false;
    
private:
    
    float calcCurrentAngle() {
        
        float x = pos.z - pivot.z;
        float y = pos.x - pivot.x;
        
        return fmod(atan2(y, x) + PI * 2, PI * 2);
    }
    
    ofVec3f     pos;
    int         currentFrame;
    string      currentSceneName;
    
    
    // data
    typedef struct {
        vector<float> kf;
        int startFrame;
        int endFrame;
    } SceneData;
    
    map<string, SceneData> data;
    
    // settings
    float       radius = 2;
    float       startAngle, endAngle;
    ofVec3f     pivot;

    
};
