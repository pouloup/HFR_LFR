/***************************************************************************
 *
 * TODO: Project Title
 *
 * Author: Florent Guiotte <guiotte.florent@gmail.com>
 *
 **************************************************************************/

#include <iostream>
#include "HFR2LFRTemporalSobel.h"
using namespace std;

int main( int argc, char* argv[] )
{
    if (argc <= 2) return 2; // TODO: usage
    string HFRVideoPath = argv[1];
    string LFRVideoPath = argv[2];

    HFR2LFRConverter * converter = new HFR2LFRTemporalSobel(HFRVideoPath, LFRVideoPath);
    converter->initialize();
    converter->convert(120, 24);
    
    return 0;
}


