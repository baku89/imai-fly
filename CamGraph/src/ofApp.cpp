#include "ofApp.h"

#include "ofxAdvancedXmlSettings.h"
#include "ofBaku.h"
#include "ImOf.h"
#include "WindowUtils.h"

#define GUIDE_PATH "/Users/baku/Dropbox/fly_guide/"

#define GUIDE_STEP 20

//--------------------------------------------------------------
void ofApp::setup(){
    
    //ofEnableSmoothing();
    ofEnableAlphaBlending();
    ofBackground(0);
	
	oscVt.setup(8000);
	oscDf.setup(1234);
	
	grabCam.setFixUpDirectionEnabled(true);
	grabCam.setPosition(.5, .5, 1);
	grabCam.lookAt(ofVec3f());
    grabCam.setLensOffset(ofVec2f(0, -0.5));
    grabCam.setNearClip(0.001);
    grabCam.setFarClip(10.0);
    grabCam.setScrollZoomEnabled(true);

	
	calibOrigin.set(0, 0, 0);
	calibAxisX.set(1, 0, 0);
	calibAlt.set(0, 0, 1);
	
	// setup gui
    ImOf::SetFont();
	gui.setup();
    ImOf::SetStyle();
    
    font.load("FiraCode-Regular.ttf", 16);
    
    yawGraph.setColor(ofColor(0, 255, 255));
    pitchGraph.setColor(ofColor(255, 0, 0));
    yGraph.setColor(ofColor(0, 255, 0));
    speedGraph.setColor(ofColor(255, 255, 0));
    
    yawGraph.setMarginY(3, 3);
    pitchGraph.setMarginY(3, 3);
    yGraph.setMarginY(.2, .2);
    speedGraph.setMarginY(0, .1);
    
    dirMesh.setMode(OF_PRIMITIVE_LINES);
    dirMesh.addVertex(ofVec3f(0, 0, 0));
    dirMesh.addVertex(ofVec3f(0, 0, 0.15));
    dirMesh.addColor(ofFloatColor(.5, .5, .5, 1));
    dirMesh.addColor(ofFloatColor(.5, .5, .5, 0));
    
    dirActiveMesh.setMode(OF_PRIMITIVE_LINES);
    dirActiveMesh.addVertices(dirMesh.getVertices());
    dirActiveMesh.addColor(ofFloatColor(1, 0, 1, 1));
    dirActiveMesh.addColor(ofFloatColor(1, 0, 1, 0));
    
    camHeightMesh.setMode(OF_PRIMITIVE_LINES);
	
	// load settings
	ofxAdvancedXmlSettings settings;
	settings.load("settings.xml");
    
    string currentSceneName;
	
	enableAutoCam    = settings.getValue("enableAutoCam", false);
    autoCamDistance  = settings.getValue("autoCamDistance", 1.0);
    sampleFramesMin  = settings.getValue("sampleFramesMin", -30);
    sampleFramesMax  = settings.getValue("sampleFramesMax", 0);
    baseHeight       = settings.getValue("baseHeight", 0.0);
    windowOnTop      = settings.getValue("windowOnTop", true);
	showRawPose      = settings.getValue("showRawPose", false);
    showImGui        = settings.getValue("showImGui", true);
    
    currentFrame     = settings.getValue("currentFrame", 0);
    currentSceneName = settings.getValue("currentSceneName", "");
	
	calibOrigin = settings.getValue("calibOrigin",	ofVec3f(0, 0, 0));
	calibAxisX	= settings.getValue("calibAxisX",	ofVec3f(1, 0, 0));
	calibAlt	= settings.getValue("calibAlt",		ofVec3f(0, 0, 1));
	
	vtCalib		= settings.getValue("vtCalib", ofMatrix4x4());
	dirCalib	= settings.getValue("dirCalib", ofMatrix4x4());
    
    ofPoint windowPos = settings.getValue("windowPos", ofVec2f(ofGetWindowPositionX(), ofGetWindowPositionY()));
    ofPoint windowSize = settings.getValue("windowSize", ofGetWindowSize());
    
    ofSetWindowPosition(windowPos.x, windowPos.y);
    ofSetWindowShape(windowSize.x, windowSize.y);
    WindowUtils::setTitlebarTransparent(true);
    WindowUtils::setWindowOnTop(windowOnTop);

    scene.load(currentSceneName);
    guide.path = GUIDE_PATH + currentSceneName + ".png";
}

//--------------------------------------------------------------
void ofApp::exit() {
	
	ofxAdvancedXmlSettings settings;
    
    settings.setValue("windowPos", ofVec2f(ofGetWindowPositionX(), ofGetWindowPositionY()));
    settings.setValue("windowSize", ofGetWindowSize());
	
	settings.setValue("enableAutoCam", enableAutoCam);
    settings.setValue("autoCamDistance", autoCamDistance);
    settings.setValue("sampleFramesMin", sampleFramesMin);
    settings.setValue("sampleFramesMax", sampleFramesMax);
    settings.setValue("baseHeight", baseHeight);
    settings.setValue("windowOnTop", windowOnTop);
	settings.setValue("showRawPose", showRawPose);
    settings.setValue("showImGui", showImGui);
    
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
    
    scene.setDuration(currentFrame + 1);
	
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
    
    static FrameData *fd, *prevFd;
    
    prevFd = scene.getPrev(currentFrame);
    
	vtPose = dirCalib * (vtRawPose * vtCalib);
    vtPos = vtPose.getTranslation();
    vtRot = vtPose.getRotate().getEuler();
    vtSpeed = (vtPos - prevFd->pos).length();
    
    while (vtRot.y - prevFd->yaw() > 180) {
        vtRot.y -= 360;
    }
    
    while (vtRot.y - prevFd->yaw() < -180) {
        vtRot.y += 360;
    }
    
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
        
        // TODO: Fix some issue
        if (scene.getName() != sceneName) {
            scene.save();
            scene.load(sceneName);
            guide.path = GUIDE_PATH + sceneName + ".png";
            guide.lastModified = -1;
            
            currentFrame = 1;
        }
		
		if (address == "/dragonframe/position") {
			
			// TODO: Set currentFrame on startup somehow
			currentFrame = frame;
		
		} else if (address == "/dragonframe/shoot") {
			
            FrameData *fd = scene.get(frame);
			fd->pos = vtPose.getTranslation();
            fd->rot = vtRot;
            fd->speed = vtSpeed;
			fd->empty = false;
            
            scene.save(true);
			
		} else if (address == "/dragonframe/cc") {
			
			string filePath = m.getArgAsString(2);
            string dirPath = ofFilePath::getEnclosingDirectory(filePath, false);
			
			scene.get(frame)->hash = ofGetFileHash(filePath);
            scene.setDirPath(dirPath);
			
		} else if (address == "/dragonframe/conform") {
			
            scene.conform();

        } else if (address == "/dragonframe/delete") {
            
            scene.get(frame)->empty = true;
        }
	}
    
    // check if the guide image has updated
    {
        static int lm;
        if (filesystem::exists(guide.path)) {
            lm = filesystem::last_write_time(guide.path);
            
            if (lm != guide.lastModified) {
                guide.lastModified = lm;
                guide.img.allocate(256, 256, OF_IMAGE_COLOR_ALPHA);
                guide.img.load(guide.path);
                ofLogNotice() << "Guide Image has updated";
            }
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw() {
	
    static ofPolyline camSpline;
    static FrameData* prevFd;
    
    // get prev frame data
    prevFd = scene.getPrev(currentFrame);
    
    if (enableAutoCam) {
        
        // get prev position
        static ofVec3f      prevPos;
        prevPos = prevFd->pos;
        
        static ofVec3f rp; // relative position from prevPos
        rp = grabCam.getGlobalTransformMatrix().getRotate() * ofVec3f(0, 0, autoCamDistance);
        
        grabCam.setPosition(prevPos + rp);
    }
	
	ofPushStyle();
	
    grabCam.begin();
	
	ofDisableDepthTest();
    ofPushMatrix();
    {
        ofTranslate(ofPoint(0, baseHeight));
        ofPushMatrix();
        {
            ofRotateZ(90);
            ofSetColor(64);
            ofDrawGridPlane(0.2f, 15.0f, false);
            
            ofSetColor(128);
            ofDrawGridPlane(1.0f, 3.0f, false);
        }
        ofPopMatrix();
        
        ofPushStyle();
        ofSetLineWidth(2);
        ofDrawAxis(1.0f);
        ofPopStyle();
    }
    ofPopMatrix();
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
        camHeightMesh.clear();
        
		yawGraph.clear();
		pitchGraph.clear();
        yGraph.clear();
        speedGraph.clear();
        
		ofSetColor(255);
		
		for (int f = 0; f < scene.getDuration(); f++) {
            
            static ofVec3f  pos, rot;
            static float    speed;
            
			FrameData       *fd = scene.get(f);
            bool            isCurrentFrame = f == currentFrame;
            bool            isInSampleRange = currentFrame + sampleFramesMin <= f && f <= currentFrame + sampleFramesMax;
			
			if (!isCurrentFrame && fd->empty) {
				continue;
			}
            
            if (isCurrentFrame) {
                pos = vtPos;
                rot = vtRot;
                speed = vtSpeed;
            } else {
                pos = fd->pos;
                rot = fd->rot;
                speed = fd->speed;
            }
			
			ofPushMatrix(); ofTranslate(pos);
            ofRotateY(rot.y); ofRotateX(rot.x); ofRotateZ(rot.z);
			{
                if (isCurrentFrame) ofSetColor(255, 0, 255);
                else if (fd == prevFd) ofSetColor(201, 136, 97);
                else ofSetColor(255);

				ofDrawSphere(0, 0, 0, 0.0025);
                
                if (isCurrentFrame) dirActiveMesh.draw();
                else dirMesh.draw();
                
                if (isInSampleRange) {
                    ofDrawBitmapString(ofToString(f), 0.0, 0.01, 0.0);
                }
                
			}
			ofPopMatrix();
			
			camSpline.addVertex(pos);
            
            camHeightMesh.addVertex(pos);
            camHeightMesh.addVertex(ofVec3f(pos.x, baseHeight, pos.z));
			
			// add graph
            if (isInSampleRange) {
                yawGraph.addValue(f,    rot.y,  !isCurrentFrame);
                pitchGraph.addValue(f,  rot.x,  !isCurrentFrame);
                yGraph.addValue(f,      pos.y,  !isCurrentFrame);
                speedGraph.addValue(f,  speed,  !isCurrentFrame);
            }
			
		}
		
		ofSetColor(255);
		camSpline.draw();
        
        ofSetColor(0, 64, 128);
        camHeightMesh.draw();
		
	}
	if (showRawPose) { ofPopMatrix(); }
	
    grabCam.end();
    
    // draw HUD
    ofPushMatrix();
    ofTranslate(0, ofGetHeight() / 2);
    {
        static float vw, vh, xmin, xmax, ct, cx;
        static ofRectangle rect;
        
        vw = ofGetWidth();
        vh = ofGetHeight() / 2;
        
        rect.set(0, 0, vw, vh);
        
        xmin = currentFrame + sampleFramesMin;
        xmax = currentFrame + sampleFramesMax;
        
        // fill black
        ofSetColor(0, 0, 0);
        ofDrawRectangle(rect);
        
        // draw playbar
        ct = (currentFrame - xmin) / (xmax - xmin);
        cx = rect.getMinX() + rect.width * ct;
        
        ofPushStyle();
        ofSetColor(255, 0, 0);
        ofSetLineWidth(3);
        ofDrawLine(cx, 0, cx, ofGetHeight());
        ofPopStyle();
        
        // draw graph
        yawGraph.setRangeX(xmin, xmax);
        pitchGraph.setRangeX(xmin, xmax);
        yGraph.setRangeX(xmin, xmax);
        speedGraph.setRangeX(xmin, xmax);
        speedGraph.setRangeY(0, .3);
        
        yawGraph.draw(rect);
        pitchGraph.draw(rect);
        yGraph.draw(rect);
        speedGraph.draw(rect);
        
        
        // guide image
        if (guide.img.isAllocated()) {
            
            static float step, scale;
            
            step = vw / (xmax - xmin);
            scale = step / GUIDE_STEP;
            
            ofEnableBlendMode(OF_BLENDMODE_SCREEN);
            ofSetColor(255, 255, 255, 192);
            
            ofPushMatrix();
            ofScale(scale, vh / guide.img.getHeight());
            ofTranslate(-xmin * GUIDE_STEP, 0);
            
            guide.img.draw(0, 0);
            
            ofPopMatrix();
            ofPopStyle();
        }
    }
    ofPopMatrix();
    
	// visible
	if (!trackerVisible) {
		ofSetColor(255, 0, 0, 128);
		ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
	}
	
	ofPopStyle();
	
	drawImGui();
}

//--------------------------------------------------------------
void ofApp::drawImGui() {
    
    if (showImGui) {
	
        static bool calibPtChanged;
        calibPtChanged = false;
        
        static bool p_open = true;
        
        gui.begin();
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::Begin("Control", &p_open, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);
        
        ImGui::SliderFloat("Cam Distance", &autoCamDistance, 0.1, 2.0);
        
        ImGui::SliderFloat("Base Height", &baseHeight, -1, 1);
        
        ImGui::DragIntRange2("Sample Frames", &sampleFramesMin, &sampleFramesMax, 1, -120, 120, "%.f");
        
        
        if (ImGui::Button("Confirm Manually")) {
            scene.conform();
        }
        
        if (ImGui::Button("Save Backup")) {
            scene.save(true);
        }
        
        if (ImGui::Checkbox("Window on Top", &windowOnTop)) {
            WindowUtils::setWindowOnTop(windowOnTop);
        }
        
        ImGui::Checkbox("Show Raw Pose", &showRawPose);
        
        ImGui::Checkbox("Enable Auto Cam", &enableAutoCam);
        
        if (ImGui::TreeNode("Calibration")) {
        
            if (ImGui::Button("Set Origin")) {
                
                calibOrigin = vtRawPose.getTranslation();
                calibPtChanged = true;
            }
            
            ImGui::SameLine();
            
            if (ImGui::Button("Set Axis X")) {
                
                calibAxisX = vtRawPose.getTranslation();
                calibPtChanged = true;
            }
            
            ImGui::SameLine();
            
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
            
            ImGui::SameLine();
            
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
        
        ImGui::End();
        
        gui.end();
    }
    
    static string frameInfo;
    static ofRectangle rect;
    
    frameInfo = scene.getName() + " - " + ofToString(currentFrame, 4, '0');
    
    rect = font.getStringBoundingBox(frameInfo, 0, 0);
    rect.x = ofGetWidth() / 2 - rect.width / 2;
    rect.y = ofGetHeight() - rect.height - 5;
    
    ofSetColor(255, 255, 255, 128);
    font.drawString(frameInfo, rect.x, rect.y);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

    
    float angle = 4;
    
    switch (key) {

        case '4':
            grabCam.rotate(-angle, 0, 1, 0);
            break;
        case '6':
            grabCam.rotate(+angle, 0, 1, 0);
            break;
        case '8':
            grabCam.rotate(-angle, grabCam.getXAxis());
            break;
        case '2':
            grabCam.rotate(+angle, grabCam.getXAxis());
            break;
        case '7':
            autoCamDistance /= 1.05;
            if (autoCamDistance < .1) {
                autoCamDistance = .1;
            }
            break;
        case '0':
            scene.conform();
            break;
        case '1':
            autoCamDistance *= 1.05;
            break;
//        case '-':
//            numSampleFrames = max(1, numSampleFrames - 1);
//            break;
//        case '+':
//            numSampleFrames = min(60, numSampleFrames + 1);
//            break;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

    switch (key) {
        case 'h':
        case '.':
            showImGui = !showImGui;
            break;
        case 'm':
            scene.conform();
            break;
            
    }
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
