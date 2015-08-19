#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include <kinect.h>
#include <Windows.h>

const int kinectWidth = 1920;
const int kinectHeight = 1080;

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		//Sencer
		IKinectSensor *pSensor;

		//Source
		IColorFrameSource *pColorSource;

		//Reader
		IColorFrameReader *pColorReader;
		
		ofImage colorImage;

		IFrameDescription* colorDescription;
		int colorWidth, colorHeight;
		unsigned int colorBytesPerPixels;
};
