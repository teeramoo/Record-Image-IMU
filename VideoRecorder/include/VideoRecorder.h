//
// Created by teeramoo on 11/11/2560.
//

#ifndef RECORDVIDEO_RECORDVIDEO_H
#define RECORDVIDEO_RECORDVIDEO_H

#include "opencv2/opencv.hpp"
#include <iostream>
#include <stdio.h>
#include <ctime>
#include <boost/thread.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/filesystem.hpp>
#include <csignal>
#include <errno.h>

#include <boost/lexical_cast.hpp>
#include "chrono"


using namespace std;
using namespace cv;
using namespace boost;
class VideoRecorder {
public:

    //function declaration
    VideoRecorder(bool _debug, int _imageWidth, int _imageHeight, double _videoFPS,
                  string _imFormat, string _filePath, int _numCamera);
    VideoRecorder();
    ~VideoRecorder();

    bool findCameras();
    int findOneCamera(int _maxNum);
    void start();
    int record();
    void configCameras();

    void stop();

    //variable declaration
private:

    //variable declaration
    bool debug;
    VideoCapture camera1, camera2;
    Mat mImage1, mImage2;
    int imageWidth, imageHeight;

    //    VideoWriter videoWriter1, videoWriter2;

    string filePath;

    //    string videoTimestampFile;

    //int videoCodec;
    string imageFormat;
    vector<int> imageParams;
    double videoFPS;
    boost::posix_time::ptime const referencedTime;
    int numCamera;
    bool isStop;
    boost::thread threadVideoRecorder;

};



#endif //RECORDVIDEO_RECORDVIDEO_H
