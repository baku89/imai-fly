#pragma once

#include "ofMain.h"
#include "ofxGrabCam.h"
#include "ofxOsc.h"
#include "ofxImGui.h"

class Frame {
	
public:
	
	Frame() {
		
	}
	
	bool			empty = true;
	ofVec3f			pos;
	ofVec3f			rot;
	string			hash = "";
	string			filePath = "";
};

class ofApp : public ofBaseApp{

public:
	void setup();
	void exit();
	void update();
	void draw();
	
	void drawImGui();
	
	void loadCurrentScene();
	void saveCurrentScene();
	void sortCurrentScene();

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
	ofxOscReceiver		oscVt, oscDf;
	
	// data
	vector<Frame*>		sheet;
	string				sheetDirPath = "";
	
	// settings
	ofMatrix4x4			vtCalib, dirCalib;
	ofVec3f				calibOrigin, calibAxisX, calibAlt;
	bool				showRawPose;
	string				currentSceneName;
	int					currentFrame;
	
	// status
	ofMatrix4x4			vtRawPose, vtPose;
	bool				trackerVisible = false;
	
	// gui
	ofxImGui::Gui		gui;
	ofPolyline			camSpline;

};
