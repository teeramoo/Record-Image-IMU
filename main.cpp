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

    if(argc != 11) {
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
    int recordTime = stoi(string(argv[9]));
    int iIMU = stoi(string(argv[10]));

    bool bIMU;
    if(iIMU == 1)
        bIMU = true;
    else
        bIMU = false;

    // load Output file path, uartPort

    pt::ptime now = pt::second_clock::local_time();
    bool debug = true;

    VideoRecorder vidRecorder(debug, imageWidth, imageHeight, fps, imFormat, filePath, numCamera);
    MAVLinkControl mavControl(uartPort, baudRate, filePath);

    if(iIMU)
        cerr << "iIMU =1" <<endl;
    if(!iIMU)
        cerr << "iIMU =0" <<endl;

    if(numCamera >0)
        vidRecorder.start();
    if(bIMU)
        mavControl.start();

    // record until 'ENTER' button is pressed
    if(recordTime == 0) {

        while(cin.get() != '\n')
            cout << "Press ENTER to end the recording." << endl;


    } else {

        auto start = std::chrono::high_resolution_clock::now();
        unsigned int elapseTime;
        int minutes = 0;
        do{

            auto currentTime = std::chrono::high_resolution_clock::now();
            elapseTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - start).count();

            if(elapseTime > (minutes +1)*60*10E2) {
                cout << minutes +1 << " minutes passed." << endl;
                cout << "elapseTime is : " << elapseTime << " milliseconds."<< endl;
                minutes++;
            }


        }while( elapseTime < recordTime * 10E2);


    }

    cout << "stop recording..." << endl;


    if(numCamera >0)
        vidRecorder.stop();

    if(bIMU)
        mavControl.stop();

    cout << "recording done." << endl;

    return 0;
}

