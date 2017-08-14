#include "ofApp.h"

#include "ofBaku.h"

//--------------------------------------------------------------
void ofApp::setup(){
	
	ofEnableSmoothing();
	ofBackground(20, 20, 20);
	
	oscVt.setup(8000);
	
	cam.setFixUpDirectionEnabled(true);
	
	cam.setPosition(.5, .5, 1);
	cam.lookAt(ofVec3f());
	
	calibOrigin.set(0, 0, 0);
	calibAxisX.set(1, 0, 0);
	calibAlt.set(0, 0, 1);
	
	// setup gui
	gui.setup();

}

//--------------------------------------------------------------
void ofApp::update(){
	
	while (oscVt.hasWaitingMessages()) {
		ofxOscMessage m;
		oscVt.getNextMessage(m);
		
		static string address;
		address = m.getAddress();
		
		if (address == "/vivetracker/position") {
			
			static float x, y, z;
			
			x = m.getArgAsFloat(0);
			y = m.getArgAsFloat(1);
			z = m.getArgAsFloat(2);
			
			vtRawPose.setTranslation(x, y, z);
			
			//obfLogNotice() << x << " " << y << " " << z;
			
		} else if (address == "/vivetracker/quaternion") {
			
			static ofQuaternion quat;
			static float w, x, y, z;
			
			w = m.getArgAsFloat(0);
			x = m.getArgAsFloat(1);
			y = m.getArgAsFloat(2);
			z = m.getArgAsFloat(3);
			
			quat.set(x, y, z, w);
			
			vtRawPose.setRotate(quat);
		
		} else if (address == "/vivetracker/visible") {
			
			trackerVisible = m.getArgAsBool(0);
			
			ofLogNotice() << trackerVisible;
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	
	ofPushStyle();
	ofEnableAlphaBlending();
	
	cam.begin();
	
	ofDisableDepthTest();
	ofPushMatrix();
	{
		ofRotateZ(90);
		ofSetColor(255);
		ofDrawGridPlane(0.2f, 30.0f, true);
	}
	ofPopMatrix();
	
	ofSetLineWidth(2);
	ofDrawAxis(1.0f);
	ofEnableDepthTest();
	
	if (!showRawPose) { ofPushMatrix(); ofMultMatrix(vtCalib); }
	{
		
		// draw camera
		ofPushMatrix(); ofMultMatrix(vtRawPose);
		{
		
			
			
			ofPushMatrix(); ofMultMatrix(dirCalib);
			{
				ofDrawAxis(.2);
				
				ofSetColor(255);
				ofDrawCamera();
			}
			ofPopMatrix();
		}
		ofPopMatrix();
		
		// draw calib points
		ofSetColor(255, 0, 255);
		ofDrawSphere(calibOrigin, 0.01);
		
		ofPushStyle();
		ofSetColor(255, 0, 0);
		ofSetLineWidth(5);
		ofDrawArrow(calibOrigin, calibAxisX, 0.015);
		ofPopStyle();
		
		
		ofSetColor(0, 255, 255);
		ofDrawSphere(calibAlt, 0.01);

		
	}
	if (!showRawPose) { ofPopMatrix(); }
	
	cam.end();
	
	
	// visible?
	if (!trackerVisible) {
		ofSetColor(255, 0, 0, 128);
		ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
	}
	
	ofPopStyle();
	
	drawImGui();
}

//--------------------------------------------------------------
void ofApp::drawImGui() {
	
	
	static bool calibPtChanged;
	calibPtChanged = false;
	
	
	gui.begin();
	
	
	if (ImGui::Checkbox("Show Raw Pose", &showRawPose));
	
	if (ImGui::Button("Set Origin")) {
		
		calibOrigin = vtRawPose.getTranslation();
		calibPtChanged = true;
	}
	
	if (ImGui::Button("Set Axis X")) {
		
		calibAxisX = vtRawPose.getTranslation();
		//calibAxisX = calibOrigin + (calibAxisX - calibOrigin).normalize();
		
		calibPtChanged = true;
	}
	
	if (ImGui::Button("Set Alt")) {
		
		calibAlt = vtRawPose.getTranslation();
		calibPtChanged = true;
	}
	
	static ofMatrix4x4 vtOffset;
	
	if (calibPtChanged) {
		
		// init
		vtOffset.set(1, 0, 0, 0,
					 0, 1, 0, 0,
					 0, 0, 1, 0,
					 0, 0, 0, 1);
		
		
		ofVec3f axisX = (calibAxisX - calibOrigin).normalize();
		ofVec3f axisZ = (calibAlt - calibOrigin).normalize();
		ofVec3f axisY = axisZ.getCrossed(axisX).normalize();
		
		axisZ = axisX.getCrossed(axisY).normalize();
		
		
		vtOffset._mat[0].set(axisX);
		vtOffset._mat[1].set(axisY);
		vtOffset._mat[2].set(axisZ);
		
		
		vtOffset.setTranslation(calibOrigin);
		
		vtCalib.makeInvertOf(vtOffset);
		
	}
	
	if (ImGui::Button("Set Direction")) {
		
		ofMatrix4x4 vtDir, camDir;
		ofVec4f up4 = -vtRawPose._mat[2];
		
		vtDir.set(vtRawPose);
		
		camDir.makeLookAtMatrix(vtDir.getTranslation(),			// eye
								ofVec3f(),						// center
								ofVec3f(up4.x, up4.y, up4.z));	// up vector
		
		dirCalib = camDir * vtDir.getInverse();
	}
	
	gui.end();
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
