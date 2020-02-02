#include<opencv2/opencv.hpp>
#include<opencv2/core/cuda.hpp>
using namespace cv;
std::string FILEPATH="../dataset/";
Mat frame;
inline bool getframe(){
    static int cur_cnt=0;
    ++cur_cnt;
    if(cur_cnt>20) return false;
    std::string filename=FILEPATH+"a"+std::to_string(cur_cnt)+".jpg";
    frame=imread(filename);
    return true;
}
int enable_Cuda=0;
int main(int argc,char *argv[]){

    if(argc>1) enable_Cuda=1;
    else enable_Cuda=0;

    if(enable_Cuda){
        cuda::setDevice(0);
    }

    while(getframe()){
        if(enable_Cuda){
            cuda::GpuMat frame_Cuda(frame);
            cuda::cvtColor(frame_Cuda,frame_Cuda,CV_BGR2HSV);
            frame_Cuda.download(frame);
            //imshow("gpu_hsv",frame);
            //waitKey(0);
        }else{
            cvtColor(frame,frame,CV_BGR2HSV);
        }
    }
    return 0;
}