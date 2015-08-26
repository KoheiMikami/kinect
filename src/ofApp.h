#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include <kinect.h>
#include <Windows.h>
//#include "ComPtr.h"

const int kinectWidth = 1920;
const int kinectHeight = 1080;

template<class Interface>
inline void SafeRelease( Interface *& pInterfaceToRelease )
{
	if( pInterfaceToRelease != NULL ){
		pInterfaceToRelease->Release();
		pInterfaceToRelease = NULL;
	}
}

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

		bool initKinect();

		//座標
		ICoordinateMapper *coorinateMapper;

		//Sencer
		IKinectSensor *pSensor;

		//Source
		IColorFrameSource *pColorSource;
		IBodyIndexFrameSource *pBodyIndexSource;
		IDepthFrameSource *pDepthSource;

		//Reader
		IColorFrameReader *pColorReader;
		IBodyIndexFrameReader  * pBodyIndexReader;
		IDepthFrameReader *pDepthReader;
	
		//カラーのサイズなど取得
		IFrameDescription* colorDescription;
		int colorWidth, colorHeight;
		unsigned int colorBytesPerPixels;

		//デプスのサイズなどの取得
		IFrameDescription *depthDescriprion;
		int depthWidth, depthHeight;
		unsigned int depthBufferSize;

		IFrameDescription *bodyIndexDescrip;

		ofImage colorImage;
		ofImage bodyIndexImage;
		ofImage depthImage;
};
