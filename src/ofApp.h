#pragma once

#include "ofMain.h"
#include "ofxJson.h"

struct IrCameraParams
{
    float fx; ///< Focal length x (pixel)
    float fy; ///< Focal length y (pixel)
    float cx; ///< Principal point x (pixel)
    float cy; ///< Principal point y (pixel)
    float k1; ///< Radial distortion coefficient, 1st-order
    float k2; ///< Radial distortion coefficient, 2nd-order
    float k3; ///< Radial distortion coefficient, 3rd-order
    float p1; ///< Tangential distortion coefficient
    float p2; ///< Tangential distortion coefficient
};

class ofApp : public ofBaseApp{
    
    string dir_name;
    ofDirectory dir;
    
    bool isPlaying = false;
    int current_frame = 0;

    ofTexture depthTex;
    ofFloatPixels depthPix;
    ofShader depthShader;
    
    float scale = 2;
    
    // pointcloud
    ofEasyCam ecam;
    ofVboMesh mesh;
    bool ispointcloud = false;
    IrCameraParams params;

	public:
		void setup();
		void update();
		void draw();
    
        void loadDir();
        void updateImage(int frame);
        float getDistanceAt(int x, int y);
        ofVec3f getWorldCoordinateAt(int x, int y, float z);
        void savePlyFiles();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
};
