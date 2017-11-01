#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetVerticalSync(true);
    ofSetFrameRate(30);
    
    depthShader.load("shader/depth_shader");
    
    mesh.setUsage(GL_DYNAMIC_DRAW);
    mesh.setMode(OF_PRIMITIVE_POINTS);
    
    ecam.setAutoDistance(false);
    ecam.setDistance(200);
    
    ofxJson json;
    ofFile file("ir_params.json", ofFile::ReadOnly);
    file >> json;
    params.fx = json["fx"];
    params.fy = json["fy"];
    params.cx = json["cx"];
    params.cy = json["cy"];
    params.k1 = json["k1"];
    params.k2 = json["k2"];
    params.p1 = json["p1"];
    params.p2 = json["p2"];
    params.k3 = json["k3"];
    
}

//--------------------------------------------------------------
void ofApp::update(){
    if(dir_name!="" && isPlaying){
        current_frame++;
        if(current_frame==dir.getFiles().size()) current_frame=0;
        updateImage(current_frame);
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofClear(0);
    
    if(depthTex.isAllocated())
    {
        if(ispointcloud){
            if (mesh.getVertices().size()) {
                ofPushView();
                ofViewport(ofRectangle(0, 0, 512*scale, 424*scale));
                ofPushStyle();
                glPointSize(2);
                ecam.begin();
                ofDrawAxis(100);
                ofPushMatrix();
                ofTranslate(0, 0, -100);
                mesh.draw();
                ofPopMatrix();
                ecam.end();
                ofPopStyle();
                ofPopView();
            }
        }
        else{
            ofPushMatrix();
            ofScale(scale, scale);
            depthShader.begin();
            depthTex.draw(0, 0);
            depthShader.end();
            ofPopMatrix();
        }
    }
    
    ofDrawBitmapStringHighlight("Fps: " + ofToString(ofGetFrameRate()), 10, 900);
    if(ispointcloud){
        ofDrawBitmapStringHighlight("Mode: pointcloud", 10, 920);
    }
    else{
        ofDrawBitmapStringHighlight("Mode: depthImage", 10, 920);
    }
    if(dir_name!=""){
        ofDrawBitmapStringHighlight("input directory: " + dir_name, 10, 940, ofColor(0, 255, 0), ofColor(0, 0, 0));
        if(isPlaying){
            ofDrawBitmapStringHighlight("isPlaying: true (press space_key to toggle)", 10, 960, ofColor(0, 255, 0), ofColor(0, 0, 0));
            ofDrawBitmapStringHighlight("frame: " + ofToString(current_frame)+"/"+ofToString(dir.getFiles().size()), 10, 980, ofColor(0, 255, 0), ofColor(0, 0, 0));
        }
        else{
            ofDrawBitmapStringHighlight("isPlaying: false (press space_key to toggle)", 10, 960, ofColor(255, 0, 0));
            ofDrawBitmapStringHighlight("frame: " + ofToString(current_frame)+"/"+ofToString(dir.getFiles().size()), 10, 980, ofColor(255, 0, 0));
        }
    }
    else{
        ofDrawBitmapStringHighlight("input directory: PLEASE CHOOSE DIRECOTRY by PRESSING key_l", 10, 940, ofColor(255, 0, 0));
    }
}

//--------------------------------------------------------------
void ofApp::loadDir(){
    ofFileDialogResult result = ofSystemLoadDialog("", true);
    dir.open(result.filePath);
    if(dir.isDirectory()){
        dir_name = result.filePath;
        dir = dir.getSorted();
    }
    else{
        dir_name = "";
        dir.close();
        cout << "please choose directory" << endl;
    }
}

//--------------------------------------------------------------
void ofApp::updateImage(int frame){
    if(ofLoadImage(depthPix, dir.getPath(frame))){
        depthTex.loadData(depthPix);
        mesh.clear();
        {
            int step = 2;
            int h = depthTex.getHeight();
            int w = depthTex.getWidth();
            for(int y = 0; y < h; y += step) {
                for(int x = 0; x < w; x += step) {
                    float dist = getDistanceAt(x, y);
                    if(dist > 50 && dist < 500) {
                        ofVec3f pt = getWorldCoordinateAt(x, y, dist);
                        //                        ofColor c;
                        //                        float h = ofMap(dist, 50, 200, 0, 255, true);
                        //                        c.setHsb(h, 255, 255);
                        //                        mesh.addColor(c);
                        mesh.addVertex(pt);
                    }
                }
            }
        }
    }
    else{
        dir_name = "";
        isPlaying = false;
    }
}

//--------------------------------------------------------------
float ofApp::getDistanceAt(int x, int y) {
    if (!depthPix.isAllocated()) {
        return 0.0f;
    }
    return depthPix[x + y * depthPix.getWidth()] * 0.1; // mm to cm
}

//--------------------------------------------------------------
ofVec3f ofApp::getWorldCoordinateAt(int x, int y, float z) {
    ofVec3f world;
    if (params.fx != 0 && params.fy != 0 && params.cx != 0 && params.cy != 0) {
        world.z = z;
        world.x = (x - params.cx) * z / params.fx;
        world.y = -(y - params.cy) * z / params.fy;
    }
    return world;
}

//--------------------------------------------------------------
void ofApp::savePlyFiles() {
    ofDirectory save_dir(dir_name+"_ply");
    save_dir.create();
    for(int i=0; i<dir.getFiles().size(); i++){
        updateImage(i);
        mesh.save(save_dir.getAbsolutePath()+"/"+
                  dir.getFile(i).getBaseName()+".ply");
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key=='l'){
        if(!isPlaying){
            loadDir();
            current_frame = 0;
            updateImage(current_frame);
        }
    }
    if(key==' '){
        if(dir_name!=""){
            isPlaying = !isPlaying;
        }
    }
    if(key=='p'){
        ispointcloud = !ispointcloud;
    }
    if(key=='s'){
        if(dir_name!=""){
            savePlyFiles();
        }
    }
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
