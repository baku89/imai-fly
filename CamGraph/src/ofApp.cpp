#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	
	ofEnableSmoothing();
	ofBackground(50, 50, 50);
	
	oscVt.setup(8000);
	
	cam.setFixUpDirectionEnabled(true);

}

//--------------------------------------------------------------
void ofApp::update(){
	
	while (oscVt.hasWaitingMessages()) {
		ofxOscMessage m;
		oscVt.getNextMessage(m);
		
		static string address;
		address = m.getAddress();
		
		if (address == "/vivetracker/position") {
			
			//ofQuaternion quat =
			
			static float x, y, z;
			
			x = m.getArgAsFloat(0);
			y = m.getArgAsFloat(1);
			z = m.getArgAsFloat(2);
			
			vtRawPose.setTranslation(x, y, z);
			
			
			ofLogNotice() << x << " " << y << " " << z;
			//vtRawPose.setRotate(quat);
			
			
		} else if (address == "/vivetracker/quaternion") {
			
			static ofQuaternion quat;
			static float w, x, y, z;
			
			w = m.getArgAsFloat(0);
			x = m.getArgAsFloat(1);
			y = m.getArgAsFloat(2);
			z = m.getArgAsFloat(3);
			
			/*
			 quat.makeRotate(m.getArgAsFloat(2), ofVec3f(0, 0, 1),
							m.getArgAsFloat(1), ofVec3f(1, 0, 0),
							m.getArgAsFloat(0), ofVec3f(0, 1, 0));
			*/
			quat.set(x, y, z, w);
			
			vtRawPose.setRotate(quat);
		}
	}

}

//--------------------------------------------------------------
void ofApp::draw(){
	
	cam.begin();
	
	ofPushMatrix();
	{
		ofRotateZ(90);
		ofDrawGridPlane(0.1f, 10.0f, true);
	}
	ofPopMatrix();
	
	ofDrawAxis(1.0f);
	
	ofPushMatrix();
	{
		//ofTranslate(vtRawPose.getTranslation());
		ofMultMatrix(vtRawPose);
		
		ofDrawAxis(.2);
	}
	ofPopMatrix();
	
	cam.end();

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
