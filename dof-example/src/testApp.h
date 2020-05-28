#pragma once

#include "ofMain.h"
#include "ofxDOF.h"

#include "ofxGui.h"

class testApp : public ofBaseApp{
  public:
	void setup();
	void update();
	void draw();
	
	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
	
    vector<ofBoxPrimitive> boxPrims;
	
	ofxDOF depthOfField;
	
	ofEasyCam camera;
	
    bool b_wireFrame, b_swimDof;

	ofxPanel gui;
};
