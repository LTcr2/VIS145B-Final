#include "ofApp.h"
/* Based off:
 * "An Array of Bouncing Balls"
 * by Jennifer Presto
 *
 * "Draw Lines" ofBook tutorial by Zach Lieberman
 * by Davide Prati
 *
 * "ofxRemoteUI" addon and Client
 * by Oriol Ferrer Mesiˆ of MIT
 */


//--------------------------------------------------------------
void ofApp::setup(){
    
    OFX_REMOTEUI_SERVER_SETUP();
    
    ofBackground(0,0,0);
    ofEnableAlphaBlending();
    ofSetBackgroundAuto(false);
    ofEnableSmoothing();
    ofSetFrameRate(30);
    ofSetWindowTitle("uniSketch!");
    
    for (int i = 0; i < NUMCIRCLES; i++)
    {
        
        //first position of balls: start evenly distributed, diagonally down to the right
        //circleX[i] = ofGetWindowWidth()* 0.15 * (i+1);
        //circleY[i] = ofGetWindowHeight()* 0.15 * (i+1);
        
        //balls start in random positions on the screen
        circleX[i] = ofRandom(ofGetHeight());
        circleY[i] = ofRandom(ofGetWidth());
        velX[i] = int(ofRandom(1, 4));
        velY[i] = int(ofRandom(1, 4));
    }
    
    //without declared type because it was declared float in the .h file
    circleRadius = 10;
    OFX_REMOTEUI_SERVER_SHARE_PARAM(circleRadius, 0, 100);
    OFX_REMOTEUI_SERVER_SHARE_COLOR_PARAM(circleColor);
    
    lineColor;
    OFX_REMOTEUI_SERVER_SHARE_COLOR_PARAM(lineColor);
    
    
    OFX_REMOTEUI_SERVER_LOAD_FROM_XML();
}

//--------------------------------------------------------------
void ofApp::update(){

    //collision detection: circle vs. circle
    for (int j = 0; j < NUMCIRCLES; j++)
    {
        for (int k = j + 1; k < NUMCIRCLES; k++)
        {
            if (ofDist(circleX[j], circleY[j], circleX[k], circleY[k]) < 65)
            {
                //if the circles hit going opposite directions,
                //both circles will reverse direction
                //(calculated seperately for both x and y movement)
                if ((velX[j] > 0 && velX[k] < 0 ) || (velX[j] < 0 && velX[k] > 0))
                {
                    velX[j] *= -1;
                    velX[k] *= -1;
                }
                if ((velY[j] > 0 && velY[k] < 0) || (velY[j] < 0 && velY[k] > 0))
                {
                    velY[j] *= -1;
                    velY[k] *= -1;
                }
            }
        }
    }
    
    
    //collision detection: circle vs. wall
    for (int i = 0; i < NUMCIRCLES; i++)
    {
        
        if (circleX[i] < 20 || circleX[i] > ofGetWidth() - 20)
        {
            velX[i] *= -1;
        }
        if (circleY[i] < 20 || circleY[i] > ofGetHeight() - 30)
        {
            velY[i] *= -1;
        }
        
        //update location of each circle
        circleX[i] += velX[i];
        circleY[i] += velY[i];
    }
    
    
    
/////////////////////////Collision Detection between Lines and Balls/////////////////////////////////

    //
    // Edited NATHANWADE
    
    //
    // Detect a collision only if the circle is withing a "margin" of the line vert
    // * is the velocity step is too small (less than margin), the circle might get trapped, reversing direction in the next update
    // * only the verticies of the line, not the line itself, are used to detect a collision in this example
    
    float margin = 10.0;
    vector<int> detected;
    
    for(auto &coord : line.getVertices()){
        
        for (int i = 0; i < NUMCIRCLES; i++){
            
            if ((circleX[i] < coord.x+margin && circleX[i] > coord.x - margin) && (circleY[i] < coord.y+margin && circleY[i] >coord.y - margin)){
            
                // check to see if index i has been pushed into our vector using std::find
                if(find(detected.begin(), detected.end(), i) != detected.end()) {
                    // do nothing if we've already detected a line vert collision
                } else {
                    // else, update the circles direction and velocity
                    velY[i] *= -1;
                    velX[i] *= -1;
                    detected.push_back(i);
                }
                
            }

        }
    }
    
    //update the circle positions outside the collision detection loop
    for (int i = 0; i < NUMCIRCLES; i++){
        circleX[i] += velX[i];
        circleY[i] += velY[i];
    }
    
/////////////////////////////////////////////////////////////////////////////////////////
    
    //creates some "noise" for the line that was drawn
    for(auto &vert : line.getVertices()){
        vert.x += ofRandom(-1, 1);
        vert.y += ofRandom(-1, 1);
    }
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    //took out background so it doesn't update and the circles draw lines
    //ofBackgroundGradient(ofColor::gray,ofColor(30, 10, 30), OF_GRADIENT_CIRCULAR);
    
    //
    // clear background for debugging
    ofBackground(0);
    
    for(int i = 0; i<NUMCIRCLES; i++)
    {
        //following line sets color of each circle a different shade of initial color
        //c = ofColor::fromHsb(colorScheme, 255, (255/NUMCIRCLES+1) * (i+1));
        
        ofSetColor(circleColor);
        ofFill();
        ofDrawCircle(circleX[i], circleY[i], circleRadius);
    }
    
    //writes instruction at the top of the screen
    ofSetColor(255);
    ofDrawBitmapString("Draw something! :)", 10, 20);
    
    //draw lines
    ofSetColor(lineColor);
    line.draw();
    
    
    //
    //visually debug the collision radius around line verts
    float margin = 10.0;
    for(auto &coord : line.getVertices()){
        ofNoFill();
        ofSetColor(0,0,255);
        ofDrawRectangle(coord.x-margin/2, coord.y-margin/2, margin, margin);
    }


}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    //keypressed examples for changing colorScheme
    /*
    if (key == 'r' || key == 'R'){
        colorScheme = 0;
    }
    if (key == 'y' || key == 'Y'){
        colorScheme = 40;
    }
    if (key == 'g' || key == 'G'){
        colorScheme = 85;
    }
    if (key == 'b' || key == 'B'){
        colorScheme = 170;
    }
     */
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
    ofPoint pt;
    pt.set(x,y);
    line.addVertex(pt);

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
    line.clear();

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
