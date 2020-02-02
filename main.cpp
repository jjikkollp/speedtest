#include<opencv2/opencv.hpp>
#include<opencv2/core/cuda.hpp>
using namespace cv;
std::string FILEPATH="../dataset/";
std::string FILEPATH2="../dataout/";
Mat frame;
inline bool getframe(){
    static int cur_cnt=0;
    ++cur_cnt;
    if(cur_cnt>20) return false;
    std::string filename=FILEPATH+"a"+std::to_string(cur_cnt)+".jpg";
    frame=imread(filename);
    return true;
}
inline void writeframe(){
    static int cur_cnt=0;
    ++cur_cnt;
    std::string filename=FILEPATH2+"a"+std::to_string(cur_cnt)+".jpg";
    imwrite(filename,frame);
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
            clock_t st=clock();
            cuda::cvtColor(frame_Cuda,frame_Cuda,CV_BGR2HSV);
            clock_t ed=clock();
            fprintf(stderr,"%d\n",ed-st);
            frame_Cuda.download(frame);
            writeframe();
        }else{
            clock_t st=clock();
            cvtColor(frame,frame,CV_BGR2HSV);
            clock_t ed=clock();
            fprintf(stderr,"%d\n",ed-st);
            writeframe();
        }
    }
    return 0;
}