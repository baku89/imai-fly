#include "ofBaku.h"

string ofGetFileHash(const string& filePath) {
	
	string hash = ofSystem("md5 -q " + filePath);
	hash.erase(std::remove(hash.begin(), hash.end(), '\n'), hash.end());
	
	return hash;
}

void ofDrawCamera () {
	
	static ofMesh cameraMesh;
	static bool initialized = false;
	
	if (!initialized) {
		initialized = false;

		// create camera
		
		vector<ofVec3f> vs;
		
		vs.push_back(ofVec3f( 12.5, 12.5, 12.5));
		vs.push_back(ofVec3f( 12.5, 12.5, 62.5));
		vs.push_back(ofVec3f(-12.5, 12.5, 12.5));
		vs.push_back(ofVec3f(-12.5, 12.5, 62.5));
		
		vs.push_back(ofVec3f( 12.5, -12.5, 12.5));
		vs.push_back(ofVec3f( 12.5, -12.5, 62.5));
		vs.push_back(ofVec3f(-12.5, -12.5, 12.5));
		vs.push_back(ofVec3f(-12.5, -12.5, 62.5));
		
		vs.push_back(vs[0]); vs.push_back(vs[2]);
		vs.push_back(vs[1]); vs.push_back(vs[3]);
		vs.push_back(vs[4]); vs.push_back(vs[6]);
		vs.push_back(vs[5]); vs.push_back(vs[7]);
		
		vs.push_back(vs[0]); vs.push_back(vs[4]);
		vs.push_back(vs[1]); vs.push_back(vs[5]);
		vs.push_back(vs[2]); vs.push_back(vs[6]);
		vs.push_back(vs[3]); vs.push_back(vs[7]);
		cameraMesh.addVertices(vs);
		
		vs.clear();
		
		vs.push_back(ofVec3f(   11,   11,    0));
		vs.push_back(ofVec3f( 3.75, 3.75, 12.5));
		vs.push_back(ofVec3f(  -11,   11,    0));
		vs.push_back(ofVec3f(-3.75, 3.75, 12.5));
		
		vs.push_back(ofVec3f(   11,  -11,    0));
		vs.push_back(ofVec3f( 3.75,-3.75, 12.5));
		vs.push_back(ofVec3f(  -11,  -11,    0));
		vs.push_back(ofVec3f(-3.75,-3.75, 12.5));
		
		vs.push_back(vs[0]); vs.push_back(vs[2]);
		vs.push_back(vs[1]); vs.push_back(vs[3]);
		vs.push_back(vs[4]); vs.push_back(vs[6]);
		vs.push_back(vs[5]); vs.push_back(vs[7]);
		
		vs.push_back(vs[0]); vs.push_back(vs[4]);
		vs.push_back(vs[1]); vs.push_back(vs[5]);
		vs.push_back(vs[2]); vs.push_back(vs[6]);
		vs.push_back(vs[3]); vs.push_back(vs[7]);
		
		cameraMesh.addVertices(vs);
		cameraMesh.setMode(OF_PRIMITIVE_LINES);

	}
	
	ofPushMatrix();
	ofScale(0.0015, 0.0015, 0.0015);
	cameraMesh.draw();
	ofPopMatrix();
}
