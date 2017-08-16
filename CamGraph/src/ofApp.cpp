#include "ofApp.h"

#include "ofxAdvancedXmlSettings.h"
#include "ofBaku.h"

#include "WindowUtils.h"

//--------------------------------------------------------------
void ofApp::setup(){
	
	ofEnableSmoothing();
	ofBackground(20, 20, 20);
	
	oscVt.setup(8000);
	oscDf.setup(1234);
	
	grabCam.setFixUpDirectionEnabled(true);
	grabCam.setPosition(.5, .5, 1);
	grabCam.lookAt(ofVec3f());

	
	calibOrigin.set(0, 0, 0);
	calibAxisX.set(1, 0, 0);
	calibAlt.set(0, 0, 1);
	
	// setup gui
	gui.setup();
	
	// load settings
	ofxAdvancedXmlSettings settings;
	settings.load("settings.xml");
    
    string currentSceneName;
	
	enableAutoCam    = settings.getValue("enableAutoCam", false);
    autoCamDistance  = settings.getValue("autoCamDistance", 1.0);
    numSampleFrames  = settings.getValue("numSampleFrames", 10);
    windowOnTop      = settings.getValue("windowOnTop", true);
	showRawPose      = settings.getValue("showRawPose", false);
    currentFrame     = settings.getValue("currentFrame", 0);
    currentSceneName = settings.getValue("currentSceneName", "");
	
	calibOrigin = settings.getValue("calibOrigin",	ofVec3f(0, 0, 0));
	calibAxisX	= settings.getValue("calibAxisX",	ofVec3f(1, 0, 0));
	calibAlt	= settings.getValue("calibAlt",		ofVec3f(0, 0, 1));
	
	vtCalib		= settings.getValue("vtCalib", ofMatrix4x4());
	dirCalib	= settings.getValue("dirCalib", ofMatrix4x4());
    
    
    WindowUtils::setTitlebarTransparent(true);
    WindowUtils::setWindowOnTop(windowOnTop);
	
    scene.load(currentSceneName);
}

//--------------------------------------------------------------
void ofApp::exit() {
	
	ofxAdvancedXmlSettings settings;
	
	settings.setValue("enableAutoCam", enableAutoCam);
    settings.setValue("autoCamDistance", autoCamDistance);
    settings.setValue("numSampleFrames", numSampleFrames);
    settings.setValue("windowOnTop", windowOnTop);
	settings.setValue("showRawPose", showRawPose);
	settings.setValue("currentSceneName", scene.getName());
	settings.setValue("currentFrame", currentFrame);
	
	settings.setValue("calibOrigin", calibOrigin);
	settings.setValue("calibAxisX", calibAxisX);
	settings.setValue("calibAlt", calibAlt);
	
	settings.setValue("vtCalib", vtCalib);
	settings.setValue("dirCalib", dirCalib);
	
	settings.save("settings.xml");
	
    scene.save();
}

//--------------------------------------------------------------
void ofApp::update(){
    
    scene.setDuration(currentFrame);
	
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
		}
	}
	
	vtPose = dirCalib * (vtRawPose * vtCalib);
    
    FrameData *fd = scene.get(currentFrame);
    fd->pos = vtPose.getTranslation();
    fd->rot = vtPose.getRotate().getEuler();
    fd->empty = false;
	
	while (oscDf.hasWaitingMessages()) {
		
		ofxOscMessage m;
		oscDf.getNextMessage(m);
		
		static string address;
		static int frame;
		static string sceneName;
		
		address = m.getAddress();
		frame = m.getArgAsInt(0);
		sceneName = m.getArgAsString(1);
        
        scene.setDuration(frame);
		
		if (address == "/dragonframe/position") {
			
			// TODO: Set currentFrame on startup somehow
			currentFrame = frame;
		
		} else if (address == "/dragonframe/shoot") {
			
            // TODO: Fix some issue
            if (scene.getName() != sceneName) {
                scene.save();
                scene.load(sceneName);
            }
			
            FrameData *fd = scene.get(frame);
			fd->pos.set(vtPose.getTranslation());
			fd->rot.set(vtPose.getRotate().getEuler());
			fd->empty = false;
			
		} else if (address == "/dragonframe/cc") {
			
			string filePath = m.getArgAsString(2);
            string dirPath = ofFilePath::getEnclosingDirectory(filePath, false);
			
			scene.get(frame)->hash = ofGetFileHash(filePath);
            scene.setDirPath(dirPath);
			
		} else if (address == "/dragonframe/conform") {
			
            scene.confirm();
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {
	
	static ofPolyline camSpline, yawGraph, pitchGraph, yGraph;
    
    // get prev frame data
    static FrameData* prevFd;
    
    prevFd = scene.get(currentFrame);
    
    for (int f = currentFrame - 1; f >= 1; f--) {
        if (!scene.get(f)->empty) {
            prevFd = scene.get(f);
            break;
        }
    }
	
	ofPushStyle();
	ofEnableAlphaBlending();
	
	if (enableAutoCam) {
		
		// get prev position
        static ofVec3f      prevPos;
        prevPos = prevFd->pos;
        
        static ofVec3f rp; // relative position from prevPos
        rp = grabCam.getGlobalTransformMatrix().getRotate() * ofVec3f(0, 0, autoCamDistance);
        
        grabCam.setPosition(prevPos + rp);
        grabCam.begin();
        
    }
	
	ofDisableDepthTest();
	ofPushMatrix();
	{
		ofRotateZ(90);
		ofSetColor(64);
		ofDrawGridPlane(0.1f, 30.0f, false);
		
		ofSetColor(128);
		ofDrawGridPlane(1.0f, 3.0f, true);
	}
	ofPopMatrix();
	
	ofSetLineWidth(2);
	ofDrawAxis(1.0f);
	ofEnableDepthTest();
	
	if (showRawPose) { ofPushMatrix(); ofMultMatrix(vtCalib.getInverse()); }
	{
		
		// draw camera
		ofPushMatrix(); ofMultMatrix(vtPose); ofScale(1, 1, -1);
		{
			ofSetColor(255);
			ofDrawCamera();
		}
		ofPopMatrix();
		
		// draw calib points
		if (showRawPose) {
			ofPushMatrix(); ofMultMatrix(vtCalib);
			{
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
			ofPopMatrix();
		}
		
		// draw spline
		
		camSpline.clear();
		yawGraph.clear();
		pitchGraph.clear();
        
        // calc scale of each graphs
        static float prevYaw, prevPitch, yawScale, pitchScale;
        
        prevYaw = prevFd->rot.y;
        prevPitch = prevFd->rot.x;
        
        yawScale = 10.0;
        pitchScale = 10.0;
        
        static int numFrames;
        numFrames = 0;
        
        for (int f = currentFrame - 1; f >= 1; f--) {
            FrameData *fd = scene.get(f);
            
            if (!fd->empty) {
                yawScale = max(yawScale, abs(fd->yaw() - prevYaw) * 2);
                pitchScale = max(pitchScale, abs(fd->pitch() - prevPitch) * 2);
            }
            
            if (++numFrames == numSampleFrames) {
                break;
            }
        }
        
        yawScale += 5.0;
        pitchScale += 5.0;
        
		ofSetColor(255);
        
        float gxStep = (float)ofGetWidth() / (numSampleFrames + 1);
		
		for (int f = 1; f <= scene.getDuration(); f++) {
			FrameData *fd = scene.get(f);
			
			if (fd->empty) {
				continue;
			}
			
			ofPushMatrix(); ofTranslate(fd->pos);
            ofRotateY(fd->rot.y); ofRotateX(fd->rot.x); ofRotateZ(fd->rot.z);
			{
                ofSetColor(255);
				ofDrawSphere(0, 0, 0, 0.005);
                
                ofSetColor(150);
                
                ofDrawBitmapString(ofToString(f), 0.0, 0.01, 0.0);
                ofDrawLine(ofVec3f(), ofVec3f(0, 0, 0.15));
			}
			ofPopMatrix();
			
			camSpline.addVertex(fd->pos);
			
			// add graph
            float gx = gxStep * (f - (currentFrame - numSampleFrames));
			float vh = ofGetHeight();
            
            float yawY = 1.0 - ((fd->yaw() - prevYaw) / yawScale + 0.5);
            float pitchY = 1.0 - ((fd->pitch() - prevPitch) / pitchScale + 0.5);
            
			yawGraph.addVertex(gx, vh * yawY);
			pitchGraph.addVertex(gx, vh * pitchY);
			
		}
		
		ofSetColor(255);
		camSpline.draw();
		
	}
	if (showRawPose) { ofPopMatrix(); }
	
    if (enableAutoCam) {
        grabCam.end();
    } else {
        grabCam.end();
    }
	
	ofSetColor(0, 255, 0);
	yawGraph.draw();
	for (auto& pt : yawGraph.getVertices()) {
		ofDrawCircle(pt, 4);
	}
	
	ofSetColor(255, 0, 0);
	pitchGraph.draw();
	for (auto& pt : pitchGraph.getVertices()) {
		ofDrawCircle(pt, 4);
	}
	
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
	
	ImGui::Text("Scene: %s", scene.getName().c_str());
	ImGui::Text("Current Frame: %s", ofToString(currentFrame).c_str());
	
    if (ImGui::Checkbox("Window on Top", &windowOnTop)) {
        WindowUtils::setWindowOnTop(windowOnTop);
        
    }
	
	ImGui::Checkbox("Show Raw Pose", &showRawPose);
    
    if (ImGui::Checkbox("Enable Auto Cam", &enableAutoCam)) {
        if (!enableAutoCam) {
            ofLogNotice() << "Disabled auto cam";
            grabCam.setTransformMatrix(autoCam.getGlobalTransformMatrix());
        }
    }
    
    ImGui::SliderFloat("Auto Cam Distance", &autoCamDistance, 0.1, 2.0);
    
    ImGui::SliderInt("Sample Frames", &numSampleFrames, 1, 60);
	
	
	if (ImGui::Button("Confirm Manually")) {
		
        scene.confirm();
	}
	
	if (ImGui::TreeNode("Calibration")) {
	
		if (ImGui::Button("Set Origin")) {
			
			calibOrigin = vtRawPose.getTranslation();
			calibPtChanged = true;
		}
		
		if (ImGui::Button("Set Axis X")) {
			
			calibAxisX = vtRawPose.getTranslation();
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
			
			ofMatrix4x4 vtDir, camDir, zInv;
			
			zInv.makeRotationMatrix(180, ofVec3f(0, 1, 0));
			
			vtDir.set(vtRawPose);
			
			camDir.makeLookAtMatrix(vtDir.getTranslation(),			// eye
									ofVec3f(),						// center
									-vtRawPose.getRowAsVec3f(2));	// up vector
			
			dirCalib = zInv * (camDir * vtDir.getInverse());
		}
		
		ImGui::TreePop();
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
    
    bool guiCaptured = ImGui::GetIO().WantCaptureMouse;
    grabCam.setMouseActionsEnabled(!guiCaptured);
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
