/***************************************************************************
 *
 * TODO: Project Title
 *
 * Author: Florent Guiotte <guiotte.florent@gmail.com>
 *
 **************************************************************************/

#include <opencv/cv.h>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/video/background_segm.hpp>
#include <opencv2/video/tracking.hpp>
#include <opencv2/cudaarithm.hpp>
#include <opencv2/cudaoptflow.hpp>

using namespace cv;
using namespace cv::cuda;
using namespace std;

void drawOptFlowMap (const Mat& flow, Mat& cflowmap, int step, const Scalar& color) {  
 for(int y = 0; y < cflowmap.rows; y += step)  
        for(int x = 0; x < cflowmap.cols; x += step)  
        {  
            const Point2f& fxy = flow.at< Point2f>(y, x);  
            line(cflowmap, Point(x,y), Point(cvRound(x+fxy.x), cvRound(y+fxy.y)),  
                 color);  
            circle(cflowmap, Point(cvRound(x+fxy.x), cvRound(y+fxy.y)), 1, color, -1);  
        }  
    }  

int main( int argc, char* argv[] )
{
     cout << "Begin" << endl;
	 int s=5;  
	 //global variables  
	 Mat GetImg;  
	 Mat prvs, next; //current frame  
     GpuMat gpu_prvs, gpu_next;
	   
	 char fileName[100] = "../../src/trans_x265_yuv420_crf0_1080p_120fps.mp4"; //video\\mm2.avi"; //mm2.avi"; //cctv 2.mov"; //mm2.avi"; //";//_p1.avi";  
	 VideoCapture stream1(fileName);   //0 is the id of video device.0 if you have only one camera     
	  
	 if(!(stream1.read(GetImg))) //get one frame form video  
	  return 2;  
	 resize(GetImg, prvs, Size(GetImg.size().width/s, GetImg.size().height/s) );  
	 cvtColor(prvs, prvs, CV_BGR2GRAY);  

     gpu_prvs.upload(prvs);
	  
     int cpt = 0;
     time_t tbegin, tend;
     double texec = 0;

     Ptr<FarnebackOpticalFlow> of = cuda::FarnebackOpticalFlow::create();//gpu_prvs, gpu_next, flowx, flowy);
     cout << "Enter main loop" << endl;
	 //unconditional loop     
     tbegin = time(NULL);
	 while (true) {     
		cout << "frame " << cpt++ << endl;
		
	  if(!(stream1.read(GetImg))) //get one frame form video     
	   break;  
	  //Resize  
	  resize(GetImg, next, Size(GetImg.size().width/s, GetImg.size().height/s) );  
	  cvtColor(next, next, CV_BGR2GRAY);  
	  ///////////////////////////////////////////////////////////////////  
	  GpuMat flow;  
      gpu_next.upload(next);
	  //calcOpticalFlowFarneback(gpu_prvs, gpu_next, flow, 0.5, 3, 15, 3, 5, 1.2, 0);  
      of->calc(gpu_prvs, gpu_next, flow);
	  
      gpu_prvs.download(prvs);
	  Mat cflow;  
	  cvtColor(prvs, cflow, CV_GRAY2BGR);  
	  //drawOptFlowMap(flow, cflow, 10, CV_RGB(0, 255, 0));  
	  //imshow("OpticalFlowFarneback", cflow);  
	  
	  ///////////////////////////////////////////////////////////////////  
	  //Display  
	  //imshow("prvs", prvs);  
	  //imshow("next", next);  
	  
	  if (waitKey(5) >= 0)     
	   break;  
	  
	  prvs = next.clone();  
      gpu_prvs = gpu_next.clone();
	 }  

     tend = time(NULL);
     texec = difftime(tend, tbegin);
     cout << "Time: " << texec << "s" << endl;
     cout << "Done" << endl;

    return 0;
}


