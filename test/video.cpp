#include "EBMA_lib.h"

cv::VideoCapture cam;

void time_callback(int time,void*)
{
    cam.set(cv::CAP_PROP_POS_FRAMES, time);
}


int main(int argc,char** argv)
{
   cam.open(argv[1]);
   if(!cam.isOpened())
   {
       printf("Failed to open the video\n");
        return -1;
   }
    int speed = 33;
    int frame_size = cam.get(cv::CAP_PROP_FRAME_COUNT);
    int time = 0 ;
    int countNumber = 0;

    while (1) {
        // Convert to HSV color space

        cv::Mat src;
        bool bSuccess = cam.read(src); // read a new frame from video

        if (!bSuccess) // if not success, break loop
        {
            std::cout << "can't read the frame from video file" << std::endl;
            break;
        }
        time++;



        cv::namedWindow("video",1);
        cv::createTrackbar("time","video",&time,frame_size,time_callback);

        if (src.empty())
            break;

        char kbin = cv::waitKey(speed);

        if (kbin == 13) //按下[ENTER]鍵儲存影像
        {
            std::cout << "capture" << std::endl;
            cv::imwrite(std::to_string(countNumber)+".png", src);
        countNumber++ ;
        }

        //快進幾秒
        if (kbin == 'a') {
            time = time - 70;
            cam.set(cv::CAP_PROP_POS_FRAMES, time);
        }
        //退後幾秒
        if (kbin == 'd') {
            time = time + 70;
            cam.set(cv::CAP_PROP_POS_FRAMES, time);
        }
        //減速
        if (kbin == 'e') {
            speed = speed - 3;
        }
        //加速
        if (kbin == 'w') {
            speed = speed + 3;
        }


        if(kbin == ' ')
        {
            while(true)
            {
                kbin = cv::waitKey(1);
                if(kbin == ' '|| kbin == 27)
                     break;

            }
        }

        if (kbin == 27)
            break;

   }
    cv::destroyAllWindows();
    return 0;

}


