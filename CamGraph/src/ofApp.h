#pragma once

#include "ofMain.h"
#include "ofxGrabCam.h"
#include "ofxOsc.h"
#include "ofxImGui.h"

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

	ofxGrabCam			cam;
	ofxOscReceiver		oscVt;
	
	// settings
	ofMatrix4x4			vtCalib, dirCalib;
	ofVec3f				calibOrigin, calibAxisX, calibAlt;
	bool				showRawPose;
	
	// status
	ofMatrix4x4			vtRawPose;
	bool				trackerVisible = false;
	
	// gui
	ofxImGui::Gui		gui;

};
