#pragma once

#include "ofMain.h"
#include "ofxGrabCam.h"
#include "ofxOsc.h"
#include "ofxImGui.h"

#include "Scene.h"
#include "Graph.h"

class ofApp : public ofBaseApp{

public:
	void setup();
	void exit();
	void update();
	void draw();
	
	void drawImGui();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);


	ofxGrabCam			grabCam;
	ofxOscReceiver		oscVt, oscDf;
    
	
	// data
    Scene               scene;
	
	// settings
	ofMatrix4x4			vtCalib, dirCalib;
	ofVec3f				calibOrigin, calibAxisX, calibAlt;
    int                 sampleFramesMin, sampleFramesMax;
    bool                windowOnTop;
	bool				showRawPose;
	bool				enableAutoCam;
    float               autoCamDistance;
    float               baseHeight;
	int					currentFrame;
    bool                showImGui;
	
	// status
	ofMatrix4x4			vtRawPose, vtPose;
    ofVec3f             vtPos, vtRot;
    float               vtSpeed;
	bool				trackerVisible = false;
	
	// gui
    ofxImGui::Gui		gui;
    stringstream        ss;
    ofTrueTypeFont      font;
    
    Graph               yawGraph, pitchGraph, speedGraph, yGraph;
    
    ofMesh              dirMesh, dirActiveMesh, camHeightMesh;
    
    struct {
        ofImage         img;
        int             lastModified = -1;
        string          path;
    } guide;

};
