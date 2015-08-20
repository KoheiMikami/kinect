#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetFrameRate(30);
	ofSetWindowShape(kinectWidth,kinectHeight);

	if (!initKinect()) {
		exit();
	}

	colorImage.allocate(colorWidth,colorHeight,OF_IMAGE_COLOR_ALPHA);	
}

//--------------------------------------------------------------
void ofApp::update(){
	// Frame
	IColorFrame* pColorFrame = nullptr;
	HRESULT hResult  = pColorReader->AcquireLatestFrame( &pColorFrame );
	if( SUCCEEDED( hResult ) ) {
		hResult = pColorFrame->CopyConvertedFrameDataToArray(colorHeight * colorWidth * colorBytesPerPixels, colorImage.getPixels(), ColorImageFormat_Rgba);
		colorImage.update();
	}
	SafeRelease( pColorFrame );	
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofSetColor(255);

	colorImage.draw(0, 0, colorImage.getWidth(), colorImage.getHeight());

}

bool ofApp::initKinect() {
	//sensorを取得
	HRESULT hResult = S_OK;
	hResult = GetDefaultKinectSensor(&pSensor);
	if (FAILED(hResult)) {
		 std::cerr << "Error : GetDefaultKinectSensor" << std::endl;
		return -1;
	}

	//sensorを開く
	hResult = pSensor->Open();
	if( FAILED( hResult ) ){
		std::cerr << "Error : IKinectSensor::Open()" << std::endl;
		return -1;
	}

	//sourceを取得
	hResult = pSensor->get_ColorFrameSource( &pColorSource ); 
		if( FAILED( hResult ) ){
		std::cerr << "Error : IKinectSensor::get_ColorFrameSource()" << std::endl;
		return -1;
	}

	//Readerを開く
	hResult = pColorSource->OpenReader( &pColorReader );
	if( FAILED( hResult ) ){
		std::cerr << "Error : IColorFrameSource::OpenReader()" << std::endl;
		return -1;
	}

	//サイズとか取得
	hResult = pColorSource->CreateFrameDescription(ColorImageFormat::ColorImageFormat_Rgba, &colorDescription );
	if(FAILED(hResult)){
		std::cerr << "Error : IColorFrameSource::get_FrameDescription()" << std::endl;
		return -1;
	}
	colorDescription->get_Width( &colorWidth );
	colorDescription->get_Height( &colorHeight );
	colorDescription->get_BytesPerPixel( &colorBytesPerPixels);

	return 0;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

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
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
