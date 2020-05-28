#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup() {
	gui.setup();
	gui.add(depthOfField.params);
	gui.setPosition(10, 50);

	b_wireFrame = false;
	b_swimDof = true;
	depthOfField.setup(ofGetWidth(), ofGetHeight());

	for (int i = 0; i < 100; i++) {
		//  make random box primitives
		ofBoxPrimitive newBox;
		newBox.set(ofRandom(30, 100), ofRandom(30, 100), ofRandom(30, 100));
		newBox.setPosition(ofVec3f(ofRandom(-300, 300),
			ofRandom(-300, 300),
			ofRandom(-300, 300)));
		for (int side = 0; side < 6; side++) { // colour each side
			newBox.setSideColor(side, ofColor::fromHsb(ofRandomuf() > .8 ? ofRandom(120, 130) : 128 - ofRandom(110, 128), //complementing random hues
				ofRandom(255), ofRandom(255)));
		}
		boxPrims.push_back(newBox);
	}
}

//--------------------------------------------------------------
void testApp::update() {
	if (!ofGetKeyPressed(' ')) {
		depthOfField.updateParams();
	}

	//TODO:
	//this do not updates the internal params/gui

	//swim the depth of field
	if (b_swimDof) depthOfField.setFocalDistance(ofMap(sin(ofGetElapsedTimef() / 2), -1, 1, 20, 150));
}

//--------------------------------------------------------------
void testApp::draw() {
	//ofEnableAntiAliasing();
	depthOfField.begin();

	camera.begin(depthOfField.getDimensions());

	ofPushStyle();

	for (int i = 0; i < boxPrims.size(); i++) {
		if (b_wireFrame) {
			boxPrims[i].drawWireframe();

		}
		else {
			boxPrims[i].draw();

		}
	}
	ofPopStyle();

	camera.end();

	depthOfField.end();

	//-

	if (ofGetKeyPressed(' ')) {
		depthOfField.drawFocusAssist(0, 0);
	}
	else {
		depthOfField.getFbo().draw(0, 0);
	}

	ofDrawBitmapString("f: toggle fullscreen, w: toggle wireframe, space: focusassist, s: swim DOF " + ofToString((b_swimDof ? "ON" : "OFF")) + " \nfps: " + ofToString(ofGetFrameRate()), 10, 10);

	gui.draw();

}

//--------------------------------------------------------------
void testApp::keyPressed(int key) {
	switch (key) {
	case 'f':
		ofToggleFullscreen();
		depthOfField.setup(ofGetWidth(), ofGetHeight());
		break;

	case 'w':
		b_wireFrame = !b_wireFrame;
	default:

	case 's':
		b_swimDof = !b_swimDof;
		break;
	}
}

//--------------------------------------------------------------
void testApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo) {

}
