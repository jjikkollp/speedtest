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
            //cuda::registerPageLocked(frame);//lock_mem
            clock_t st1=clock();
            cuda::GpuMat frame_Cuda(frame);
            clock_t ed1=clock();
            cuda::cvtColor(frame_Cuda,frame_Cuda,CV_BGR2HSV);
            
            clock_t st2=clock();
            frame_Cuda.download(frame);
            clock_t ed2=clock();
            fprintf(stderr,"%.6f\n",double(ed2-st2)/CLOCKS_PER_SEC);
            //cuda::unregisterPageLocked(frame);//unlock_mem
            writeframe();
        }else{
            clock_t st=clock();
            cvtColor(frame,frame,CV_BGR2HSV);
            clock_t ed=clock();
            fprintf(stderr,"%.6f\n",double(ed-st)/CLOCKS_PER_SEC);            
            writeframe();
        }
    }
    return 0;
}