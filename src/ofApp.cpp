#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetFrameRate(30);
	ofSetWindowShape(kinectWidth,kinectHeight);

	if (!initKinect()) {
		exit();
	}

	colorImage.allocate(colorWidth,colorHeight,OF_IMAGE_COLOR_ALPHA);	
	bodyIndexImage.allocate(512,424,OF_IMAGE_COLOR_ALPHA);
}

//--------------------------------------------------------------
void ofApp::update(){
	// color
	IColorFrame* pColorFrame = nullptr;
	HRESULT hResult  = pColorReader->AcquireLatestFrame( &pColorFrame );

	if( SUCCEEDED( hResult ) ) {
		hResult = pColorFrame->CopyConvertedFrameDataToArray(colorHeight * colorWidth * colorBytesPerPixels, colorImage.getPixels(), ColorImageFormat_Rgba);
		colorImage.update();
	}
	
	//bodyindex
	IBodyIndexFrame *pBodyIndexFrame  = nullptr;
	hResult = pBodyIndexReader->AcquireLatestFrame( &pBodyIndexFrame );

	
	int width = 512;
	int height  = 424;
	if (SUCCEEDED(hResult)) {
		unsigned int bufferSize = 0;
		unsigned char* buffer = nullptr;

		//bodyindexの取得
		hResult = pBodyIndexFrame->AccessUnderlyingBuffer( &bufferSize, &buffer );
		
		
		if( SUCCEEDED( hResult ) ){
			unsigned char *data = bodyIndexImage.getPixels();

			for( int y = 0; y < 424;  y++ ){
				for( int x = 0; x < 512;  x++ ){
					unsigned int  index = y * width + x;

					//非人物領域が0xff
					if( buffer[index] != 0xff ){
						data[index* 4] = 255;
						data[index * 4 + 1] = 255;
						data[index * 4 + 2] = 255;
						data[index  * 4+ 3] = 255;

					}else{
						data[index* 4] = 0;
						data[index * 4 + 1] = 0;
						data[index * 4 + 2] = 0;
						data[index  * 4+ 3] = 0;
					}
				}
				bodyIndexImage.update();
			}
		}

	}

	//解放
	SafeRelease( pColorFrame );
	SafeRelease( pBodyIndexFrame );
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofSetColor(255);

	colorImage.draw(0, 0, colorImage.getWidth(), colorImage.getHeight());

	ofSetColor(0);
	bodyIndexImage.draw(0,0,512,424);
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
	hResult = pSensor->get_BodyIndexFrameSource( &pBodyIndexSource ); 
	if( FAILED( hResult ) ){
		std::cerr << "Error : IKinectSensor::get_BodyIndexFrameSource()" << std::endl;
		return -1;
	}

	//Readerを開く
	hResult = pColorSource->OpenReader( &pColorReader );
	if( FAILED( hResult ) ){
		std::cerr << "Error : IColorFrameSource::OpenReader()" << std::endl;
		return -1;
	}

	hResult = pBodyIndexSource->OpenReader( &pBodyIndexReader ); 
	if( FAILED( hResult ) ){
		std::cerr << "Error : IBodyIndexFrameSource::OpenReader()" << std::endl;
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

	int bodyWidth = 0;
	int bodyHeight = 0;
	hResult = pBodyIndexSource -> get_FrameDescription(&bodyIndexDescrip);
	bodyIndexDescrip -> get_Width(&bodyWidth);
	bodyIndexDescrip -> get_Height(&bodyHeight);
	cout << bodyWidth << endl;
	cout << bodyHeight << endl;

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
