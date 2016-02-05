/***************************************************************************
 *
 * TODO: Project Title
 *
 * Author: Florent Guiotte <guiotte.florent@gmail.com>
 *
 **************************************************************************/

#include <iostream>
#include "HFR2LFRTemporalGradient.h"
#include "HFR2LFRSpatiotemporalGradient.h"
#include "HFR2LFRSpatiotemporalGradient2.h"
using namespace std;

int main( int argc, char* argv[] )
{
    if (argc <= 2) return 2; // TODO: usage
    string HFRVideoPath = argv[1];
    string LFRVideoPath = argv[2];

    cv::Vec4f aze(1, 2, 3, 4);
    cout << aze[1] << endl;
    //cv::Mat test(aze);
    cv::Mat test(2, 2, CV_32FC1);
    //test.reshape(0, 2);
    //test.resize(2);
    *(test.begin<float>() + 0) = aze[0];
    *(test.begin<float>() + 1) = aze[1];
    *(test.begin<float>() + 2) = aze[2];
    *(test.begin<float>() + 3) = aze[3];
    Mat ttest;
    normalize(test, ttest, 0, 1, NORM_MINMAX, CV_32FC1);
    cout << aze << endl;
    cout << test << endl;
    return 0;

    //HFR2LFRConverter * converter = new HFR2LFRTemporalGradient(HFRVideoPath, LFRVideoPath);
    //HFR2LFRConverter * converter = new HFR2LFRSpatiotemporalGradient(HFRVideoPath, LFRVideoPath);
    HFR2LFRConverter * converter = new HFR2LFRSpatiotemporalGradient2(HFRVideoPath, LFRVideoPath);
    converter->initialize();
    //converter->convert(10, 100);
    converter->convert(2000, 150000);
    
    return 0;
}


