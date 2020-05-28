
#include "ofxDOF.h"

ofxDOF::ofxDOF(){
	blurAmount = 1;
	focalRange = 50;
	focalDistance = 150;

	//params
	params.add(blurAmount_);
	params.add(focalRange_);
	params.add(focalDistance_);
	//params.add(debug_);
}

//params
void ofxDOF::updateParams() {
	setBlurAmount(blurAmount_);
	setFocalRange(focalRange_);
	setFocalDistance(focalDistance_);
}

void ofxDOF::setup(){
	setup(ofGetWidth(), ofGetHeight());
}

void ofxDOF::setup(int width, int height){
	
	dimensions = ofRectangle(0, 0, width, height);
    ofFbo::Settings dofBuffersSettings;
    dofBuffersSettings.width = width;
    dofBuffersSettings.height = height;
    dofBuffersSettings.internalformat = GL_RGB;
    dofBuffersSettings.numSamples = 0;
    dofBuffersSettings.useDepth = true;
    dofBuffersSettings.useStencil = true;
    dofBuffersSettings.depthStencilAsTexture = true;
    dofBuffersSettings.textureTarget = ofGetUsingArbTex() ? GL_TEXTURE_RECTANGLE_ARB : GL_TEXTURE_2D;
	
    dofTarget.allocate(dofBuffersSettings);
	
    swap.allocate(width, height, GL_RGB);
    scene.allocate(width, height, GL_RGB);
	
    swap.begin();
    ofClear(0,0,0,0);
    swap.end();
	
    dofTarget.begin();
    ofClear(0,0,0,0);
    dofTarget.end();
	
    dofQuad.clear();
    dofQuad.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
    dofQuad.addVertex(ofVec3f(0,0,0));
    dofQuad.addVertex(ofVec3f(0,height,0));
    dofQuad.addVertex(ofVec3f(width,0,0));
    dofQuad.addVertex(ofVec3f(width,height,0));
    
    dofQuad.addTexCoord(ofVec2f(0,0));
    dofQuad.addTexCoord(ofVec2f(0,height));
    dofQuad.addTexCoord(ofVec2f(width,0));
    dofQuad.addTexCoord(ofVec2f(width,height));
	
    if (ofIsGLProgrammableRenderer()) {
        setShaderPath("dof_gl3/");
    }
    else {
        setShaderPath("dof/");
    }
}

//default path is bin/data/dof/
void ofxDOF::setShaderPath(string path){
	shaderPath = path;
	reloadShader();
}

// you can call this if you are tweaking the shader file
void ofxDOF::reloadShader(){
	depthOfField.load(shaderPath + "dof");
	focusAssist.load(shaderPath + "doffocusassist");
}

//usually between 0 and 2 or 3
void ofxDOF::setBlurAmount(float blurAmount){
	this->blurAmount = blurAmount;
}
float ofxDOF::getBlurAmount(){
	return blurAmount;
}

// how much of the scene is in focus, smaller number is a narrower focal distance
void ofxDOF::setFocalRange(float focalRange){
	this->focalRange = focalRange;
}
float ofxDOF::getFocalRange(){
	return focalRange;
}

// where is the focal plane from the camera
void ofxDOF::setFocalDistance(float focalDistance){
	this->focalDistance = focalDistance;
}
float ofxDOF::getFocalDistance(){
	return focalDistance;
}

ofRectangle ofxDOF::getDimensions(){
	return dimensions;
}

int ofxDOF::getWidth(){
	return dofTarget.getWidth();
}

int ofxDOF::getHeight(){
	return dofTarget.getHeight();
}

ofFbo& ofxDOF::getFbo(){
	return scene;
}

void ofxDOF::begin(){
	dofTarget.begin();
	ofClear(0,0,0,0);
    ofEnableDepthTest();
}

void ofxDOF::end(){
    ofDisableDepthTest();
    dofTarget.end();
	
	//composite
	swap.begin();
	ofClear(0.0,0.0,0.0,0.0);
	
	ofPushStyle();
	ofSetColor(255);
	depthOfField.begin();
	
	depthOfField.setUniform2f("sampleOffset", blurAmount, 0);
	depthOfField.setUniform1f("focalDistance", focalDistance);
	depthOfField.setUniform1f("focalRange", focalRange);
	
    depthOfField.setUniformTexture("tex", dofTarget.getTexture(), 0);
	//depthOfField.setUniformTexture("tex", dofTarget.getTextureReference(), 0);
	depthOfField.setUniformTexture("range", dofTarget.getDepthTexture(), 1);
	
	dofQuad.draw();
	
	depthOfField.end();
	
	ofPopStyle();
	
	swap.end();
	
	scene.begin();
	ofClear(0.0,0.0,0.0,0.0);
	
	ofPushStyle();
	
	ofSetColor(255, 255, 255, 255);
	
	depthOfField.begin();
	depthOfField.setUniform2f("sampleOffset", 0, blurAmount);
    depthOfField.setUniformTexture("tex", swap.getTexture(), 0);

   // depthOfField.setUniformTexture("tex", swap.getTextureReference(), 0);
	depthOfField.setUniformTexture("range", dofTarget.getDepthTexture(), 1);
	
	dofQuad.draw();
	
	depthOfField.end();
	
	ofPopStyle();
	
	scene.end();
}


void ofxDOF::drawFocusAssist(int x, int y){
	drawFocusAssist(x,y,getWidth(),getHeight());
}

void ofxDOF::drawFocusAssist(int x, int y, int width, int height){
	drawFocusAssist(ofRectangle(x,y,width,height));
}

void ofxDOF::drawFocusAssist(ofRectangle drawRect){
	
	focusAssist.begin();
	focusAssist.setUniform1f("focalDistance", focalDistance);
	focusAssist.setUniform1f("focalRange", focalRange);
	
	dofTarget.getDepthTexture().draw(drawRect);
	
	focusAssist.end();

}
