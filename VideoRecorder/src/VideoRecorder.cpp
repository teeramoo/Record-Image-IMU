
//#include "VideoRecorder.h"
#include "../include/VideoRecorder.h"

VideoRecorder::~VideoRecorder() {}

VideoRecorder::VideoRecorder(bool _debug, int _imageWidth, int _imageHeight, double _fps,
                             string _imFormat, string _filePath, int _numCamera)
        : debug(_debug), imageWidth(_imageWidth), imageHeight(_imageHeight),
          videoFPS(_fps), imageFormat(_imFormat), filePath(_filePath), numCamera(_numCamera), isStop(false) {

    if(imageFormat == "jpeg") {
        imageParams.push_back(IMWRITE_JPEG_QUALITY);
        imageParams.push_back(95);
    }
    else if(imageFormat == "png") {
        imageParams.push_back(CV_IMWRITE_PNG_COMPRESSION);
        imageParams.push_back(3);
    }


}

void VideoRecorder::configCameras()
{
    camera1.set(CAP_PROP_FRAME_WIDTH, imageWidth);
    camera1.set(CAP_PROP_FRAME_HEIGHT, imageHeight);
    camera1.set(CAP_PROP_FPS, videoFPS);

    if(numCamera == 2)
    {
        camera2.set(CAP_PROP_FRAME_WIDTH, imageWidth);
        camera2.set(CAP_PROP_FRAME_HEIGHT, imageHeight);
        camera2.set(CAP_PROP_FPS, videoFPS);
    }
    	cout << "recording sequence of images with " << imageWidth << "x" << imageHeight << " quality at " << videoFPS << " fps." << endl;
}

void VideoRecorder::start()
{
    if(!findCameras())
    {
        cerr << "Error opening cameras." <<endl;
        throw;
    }
    configCameras();
    threadVideoRecorder = boost::thread(&VideoRecorder::record,this);

}

bool VideoRecorder::findCameras()
{
    int camIndex1 = findOneCamera(5); // Camera index 1

    if(camIndex1 == -1)
    {
        cout << "Cannot find any camera." <<endl;
        return false;
    }
    camera1 = VideoCapture(camIndex1);

    if(numCamera ==2)
    {
        int camIndex2 = findOneCamera(camIndex1-1); //Camera index 2

        if(camIndex2 == -1)
        {
            cout << "Cannot find second camera." <<endl;
            return false;
        }
        camera2 = VideoCapture(camIndex2);

    }

    return  true;
}

int VideoRecorder::findOneCamera(int _maxNum)
{
    for (int i = _maxNum; i >= 0; i--)
    {
        cout << "Try camera index : " << i << endl;
        VideoCapture checkCamera(i);
        if(checkCamera.isOpened())
        {
            cout << "Found camera index : " << i << endl;
            checkCamera.release();
            return i;
        }
        else
            checkCamera.release();
    }
    return -1;
}



int VideoRecorder::record()
{


    if(!camera1.isOpened()) {
        cerr << "Cannot open the first camera." << endl;
        return -1;
    }

    if(numCamera == 2) {

        if(!camera2.isOpened()) {

            cerr << "Cannot open the second camera." << endl;
            return -1;

        }

    }

    int frame = 0;
    ofstream vidTimeWriter;
    string timestampPath = filePath + "video_timestamp.txt";
    vidTimeWriter.open(timestampPath);

//    auto refTime = boost::posix_time::microsec_clock::local_time();
//    auto startTime = boost::posix_time::microsec_clock::local_time();

    auto refTime = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now().time_since_epoch());
    auto startTime = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now().time_since_epoch());

    int frameCount = 0;

    while (!isStop)
    {

        // check if we succeeded
        if (!camera1.read(mImage1)) {
            cerr << "ERROR! blank frame grabbed from camera 1\n";
            return -1;
        }

        if(numCamera ==2) {
            if (!camera2.read(mImage2)) {
                cerr << "ERROR! blank frame grabbed from camera 2\n";
                return -2;
            }

        }


        // encode the frame into the videofile stream
//        std::stringstream ss;
//        ss << std::fixed << std::setfill('0') << std::setw(6) << frame+1;
//        std::string imageNumber = ss.str();

//        std::string imCamera1Name = filePath + "/camera1/" +imageNumber +".jpeg" ;
//        cv::imwrite(imCamera1Name, mImage1, params );
//        mImage1.release();

//        std::string imCamera2Name = filePath + "/camera2/"+imageNumber +".jpeg" ;
//        cv::imwrite(imCamera2Name, mImage2, params);
//        mImage2.release();

        frameCount++;
      //  auto ms =  boost::chrono::duration_cast<boost::chrono::nanoseconds>(boost::chrono::high_resolution_clock::now().time_since_epoch());
        auto ms =  std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now().time_since_epoch());
//        auto ms = (boost::posix_time::microsec_clock::local_time() - referencedTime).total_microseconds();
        std::string imageName = to_string(ms.count());

        std::string imCamera1Name = filePath + "/camera1/" +imageName +"." + imageFormat ;
        cv::imwrite(imCamera1Name, mImage1, imageParams );
        mImage1.release();

        if(numCamera == 2) {
        std::string imCamera2Name = filePath + "/camera2/"+imageName +"."+ imageFormat ;
        cv::imwrite(imCamera2Name, mImage2, imageParams);
        mImage2.release();
        }


        vidTimeWriter << imageName << "," << imageName +"." + imageFormat << endl;

        if(frame % (int) videoFPS == 0) {
            auto ms2 = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now().time_since_epoch() - refTime);
        cout << "--------------------------" << endl;
        cout << "Frame number : " << frame << endl;
        cout << "time elapse : " << ms2.count()/ (double)10E8 << endl;
        cout << "FPS : " << frameCount*(double)10E8/ms2.count() << endl;

            auto timeElapse = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()- startTime);
//            cout << "time elapse : " << timeElapse/1000000.0 << endl;
            cout << "average FPS : " << (double)frame / timeElapse.count() * 10E8 << endl;
        refTime = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now().time_since_epoch());
        frameCount = 0;
        }


        frame++;

//        if (waitKey(5) >= 0)
//            break;
    }

        camera1.release();
        if(numCamera ==2)
            camera2.release();
        cout << "Release all cameras." << endl;


    vidTimeWriter.close();
    auto timeElapse = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()- startTime);
    cout << "SUMMARY" << endl;
    cout << "time elapse : " << timeElapse.count()/10E8 << endl;
    cout << "frame : " << frame << endl;
    cout << "average FPS : " << (double)frame / timeElapse.count() * 10E8 << endl;
}

void VideoRecorder::stop() {
    isStop = true;
    threadVideoRecorder.join();
}
