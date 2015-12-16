/**
 * Authors: Anthony ARCH - Pierre-Henri DAUVERGNE - Jérémy VIGOUR (ESIR)
**/

#include <ctime>

#include "HFR2LFRBlurConverter.h"
#include "HFR2LFRFusionConverter.h"
#include "HFR2LFRFlowDrawer.h"
#include "HFR2LFRDecimationConverter.h"

using namespace cv;
using namespace std;

string secondsToMin(double seconds)
{
	ostringstream ss;
	int min = (int) (seconds/60);
	ss << min << " min " << seconds-(min*60.0) << " sec";
	return ss.str();
}


int main(int argc, char **argv)
{
	int video = 0;
	int nbFrames = -1;
	int startFrame = -1;
	int method = -1;
	string HFRVideoPath;
	ostringstream LFRVideoPath;

	while(video != 1 && video != 2)
	{
		cout << "Quelle video ?" << endl;
		cout << "1. Chevaux" << endl;
		cout << "2. Composition" << endl;
		cout << "3. Sequence Florent" << endl;
		cout << "Choix : ";
		cin >> video;
	}

	if(video == 1)
	{
		//HFRVideoPath = "../../data/chevaux_1920x1080_UYVY_120fps.avi";
		HFRVideoPath = "X:/BCOM/Chevaux-90_1920x1080_120Hz_I420_8bits.avi";
		LFRVideoPath << "X:/BCOM/Chevaux-90_1920x1080_60Hz_I420_8bits_simul";
	}
	else if (video == 2)
	{
		HFRVideoPath = "../../data/composition_1920x1080_UYVY_120fps.avi";
		LFRVideoPath << "../../data/composition_1920x1080_UYVY_60fps_simul";
	}
	else if (video == 3)
	{
		HFRVideoPath = "../../data/sequence_florent_1920x1080_UYVY_120fps.avi";
		LFRVideoPath << "../../data/sequence_florent_1920x1080_UYVY_60fps_simul";
	}

	while(nbFrames < 0)
	{
		cout << "Nombre de frames LFR a genesrer ?" << endl << "(0 = toutes) : ";
		cin >> nbFrames;
	}

	while(startFrame <= 0)
	{
		cout << "Commencer a la frame : ";
		cin >> startFrame;
	}

	while(method < 1 || method > 4)
	{
		cout << "Quelle methode ?" << endl;
		cout << "1. Fusion d'images" << endl;
		cout << "2. Flou cinetique" << endl;
		cout << "3. Afficher seulement le flot optique" << endl;
		cout << "4. Simple decimation 1/2 frame." << endl;
		cout << "Choix : ";
		cin >> method;
	}

	clock_t startTime = clock();
	HFR2LFRConverter * converter = 0;

	if(method == 1)
	{
		LFRVideoPath << "fusion.avi";
		converter = new HFR2LFRFusionConverter(HFRVideoPath, LFRVideoPath.str());
	}
	else if(method == 2)
	{
		LFRVideoPath << "blur.avi";
		converter = new HFR2LFRBlurConverter(HFRVideoPath, LFRVideoPath.str());
	}
	else if(method == 3)
	{
		LFRVideoPath << "flow.avi";
		converter = new HFR2LFRFlowDrawer(HFRVideoPath, LFRVideoPath.str());
	}
	else if(method == 4)
	{
		LFRVideoPath << "decimation1_2.avi";
		converter = new HFR2LFRDecimationConverter(HFRVideoPath, LFRVideoPath.str());
	}

	converter->initialize();
	converter->convert(nbFrames, startFrame);

	double stopTime = double(clock() - startTime) / (double) CLOCKS_PER_SEC;
	cout << "Temps total : " << stopTime << " sec";
	if(stopTime >= 60.0)
	{
		cout << " (" << secondsToMin(stopTime) << ")";
	}
	cout << endl;

	delete converter;
	return 0;
}