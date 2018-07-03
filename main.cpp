//
// Created by teeramoo on 11/11/2560.
//

#include "VideoRecorder.h"
#include "mavlink_control.h"


namespace pt = boost::posix_time;

void createDirectory(string _directoryName)
{
    if(!boost::filesystem::exists(_directoryName))
    {
        boost::filesystem::create_directory(_directoryName);
        cout << "Created directory : " << _directoryName << endl;
    }

    else if(boost::filesystem::exists(_directoryName))
        cout << _directoryName << " exists." << endl;
    else
        cout << "Error creating directory : " << _directoryName << endl;

}


int main(int argc, char ** argv)
{

    if(argc != 9) {
        cout << "Usage: " << argv[0] << " <file_path> <image_width> <image_height> <imqage_fps> <number_of_cameras> <image_format>"
                                        "<uart_port> <baud_rate> " << endl;

        cout << "Note that the supported image format are jpeg and png." << endl;
        exit(-1);
    }


    string filePath = string(argv[1]);
    int imageWidth = stoi(string(argv[2]));
    int imageHeight = stoi(string(argv[3]));
    double fps = stod(string(argv[4]));
    int numCamera = stoi(string(argv[5]));
    string imFormat = string(argv[6]);

    string uartPort = string(argv[7]);
    int baudRate = stoi(string(argv[8]));

    // load Output file path, uartPort

    pt::ptime now = pt::second_clock::local_time();
    bool debug = true;

    VideoRecorder vidRecorder(debug, imageWidth, imageHeight, fps, imFormat, filePath, numCamera);
    MAVLinkControl mavControl(uartPort, baudRate, filePath);


//    cout << "hello" << endl;
    if(numCamera >0)
        vidRecorder.start();

    mavControl.start();

    while(cin.get() != '\n')
        cout << "Press ENTER to end the recording." << endl;
    cout << "stop recording..." << endl;

    if(numCamera >0)
        vidRecorder.stop();

    mavControl.stop();

    cout << "recording done." << endl;

    return 0;
}

