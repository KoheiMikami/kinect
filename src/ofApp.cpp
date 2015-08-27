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
	depthImage.allocate(512,424,OF_IMAGE_GRAYSCALE);
	kirinukiImage.allocate(colorWidth,colorHeight,OF_IMAGE_COLOR);

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


	if (SUCCEEDED(hResult)) {
		unsigned char *bodyIndexBuffer = nullptr;
		unsigned int bufferSize = 0;

		//bodyindexの取得
		hResult = pBodyIndexFrame->AccessUnderlyingBuffer( &bodyIndexBufferSize, &bodyIndexBuffer );

		if( SUCCEEDED( hResult ) ){

			for (int colorIndex = 0; colorIndex <  (colorWidth * colorHeight);  colorIndex++) {
				
				for( int y = 0; y < 424;  y++ ){
					for( int x = 0; x < 512;  x++ ){
						unsigned int  index = y * bodyIndexWidth + x;
						if( bodyIndexBuffer[index] != 0xff ){
							UINT  colorBuffer = (colorHeight * colorWidth * colorBytesPerPixels;
							DepthSpacePoint depthspace = 
						}else{
						
						}

					}
				}
			
			}

			/*
			unsigned char *bodyIndexData = bodyIndexImage.getPixels();

			for( int y = 0; y < 424;  y++ ){
			for( int x = 0; x < 512;  x++ ){
			unsigned int  index = y * bodyIndexWidth + x;

			//非人物領域が0xff
			if( bodyIndexBuffer[index] != 0xff ){
			bodyIndexData[index* 4] = 0;
			bodyIndexData[index * 4 + 1] = 0;
			bodyIndexData[index * 4 + 2] = 0;
			bodyIndexData[index  * 4+ 3] = 255;
			}else{
			bodyIndexData[index* 4] = 0;
			bodyIndexData[index * 4 + 1] = 0;
			bodyIndexData[index * 4 + 2] = 0;
			bodyIndexData[index  * 4+ 3] = 0;
			}
			}
			bodyIndexImage.update();
			}*/

		}

	}

	//Depth
	IDepthFrame *pDepthFrame = nullptr;
	hResult = pDepthReader -> AcquireLatestFrame(&pDepthFrame);
	if (SUCCEEDED(hResult)) {
		UINT16 *depthBuffer = nullptr;
		hResult = pDepthFrame -> AccessUnderlyingBuffer(&depthBufferSize,&depthBuffer);
		if (SUCCEEDED(hResult)) {
			unsigned char *depthData = depthImage.getPixels();
			for (int i = 0; i < depthBufferSize; i++) {
				depthData[i] = ((depthBuffer[i] * 255)/8000);
			}
			depthImage.update();
		}		  
	}

	//解放
	SafeRelease( pColorFrame );
	SafeRelease( pBodyIndexFrame );
	SafeRelease(pDepthFrame);
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofSetColor(255);

	colorImage.draw(0, 0, colorImage.getWidth(), colorImage.getHeight());

	ofSetColor(255);
	ofRect(0,0,bodyIndexWidth,bodyIndexHeight);
	bodyIndexImage.draw(0,0,512,424);

	depthImage.draw(0,424,512,424);

	//kirinukiImage.draw(0,0,colorImage.getWidth(), colorImage.getHeight());
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

	//座標インターフェースを取得
	hResult  = pSensor -> get_CoordinateMapper(&coorinateMapper);
	if (FAILED(hResult)) {
		std::cerr << "Error : IKinectSensor::get_CoordinateMapper()" << std::endl;
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
	hResult = pSensor->get_DepthFrameSource(&pDepthSource);
	if( FAILED( hResult ) ){
		std::cerr << "Error : IKinectSensor::get_DepthFrameSource()" << std::endl;
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

	hResult = pDepthSource -> OpenReader(&pDepthReader);
	if( FAILED( hResult ) ){
		std::cerr << "Error : IDepthSource::OpenReader()" << std::endl;
		return -1;
	}

	//カラーの色々取得
	hResult = pColorSource->CreateFrameDescription(ColorImageFormat::ColorImageFormat_Rgba, &colorDescription );
	if(FAILED(hResult)){
		std::cerr << "Error : IColorFrameSource::get_FrameDescription()" << std::endl;
		return -1;
	}
	colorDescription->get_Width( &colorWidth );
	colorDescription->get_Height( &colorHeight );
	colorDescription->get_BytesPerPixel( &colorBytesPerPixels);

	//デプスの色々取得
	hResult = pDepthSource -> get_FrameDescription(&depthDescriprion);
	if (FAILED(hResult)) {
		return -1;
	}
	depthDescriprion -> get_Width(&depthWidth);
	depthDescriprion -> get_Height(&depthHeight);
	depthDescriprion->get_BytesPerPixel(&depthBufferSize);

	//BodyIndexの色々取得
	hResult = pBodyIndexSource -> get_FrameDescription(&bodyIndexDescrip);
	if (FAILED(hResult)) {
		return -1;
	}
	bodyIndexDescrip -> get_Width(&bodyIndexWidth);
	bodyIndexDescrip -> get_Height(&bodyIndexHeight);
	bodyIndexDescrip -> get_BytesPerPixel(&bodyIndexBufferSize);

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
