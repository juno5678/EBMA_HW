#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <string.h>
#include <iostream>
#include "EBMA_lib.h"
#include "fast_BMA_lib.h"


int main(int argc,char **argv)
{
    if(argc != 3)
    {
        printf("you have to type the two picture path\n");
        return 0;
    }
    std::string current_frame_path = argv[1];
    printf("current frame path : %s\n",current_frame_path.c_str());
    std::string ref_frame_path = argv[2];
    printf("ref frame path : %s\n",ref_frame_path.c_str());
    cv::Mat cur_frame = cv::imread(current_frame_path);

    if(cur_frame.empty())
    {
        printf("%s didm't have picture\n",current_frame_path.c_str());
        return 0;
    }
    cv::Mat cur_img = cur_frame.clone();

    cv::Mat ref_frame = cv::imread(ref_frame_path);
    if(ref_frame.empty())
    {
        printf("%s didm't have picture\n",ref_frame_path.c_str());
        return 0;
    }
    cv::Mat ref_img = ref_frame.clone();

    cv::Mat predict_frame;
    cv::Mat diff_frame;
    cv::Mat L_cur_frame,L_ref_frame;

    Get_chrominance(cur_img,L_cur_frame);
    Get_chrominance(ref_img,L_ref_frame);

    //EBMA(L_cur_frame,L_ref_frame,predict_frame,6,4);
    BMA_2Dlog(L_cur_frame,L_ref_frame,predict_frame,6,4);
    printf("mse %3f\n",Cal_MSE(predict_frame,L_ref_frame));

    cv::imshow("predict_frame",predict_frame);
    cv::imshow("current frame",L_cur_frame);
    cv::imshow("ref frame",L_ref_frame);
    cv::waitKey(0);
    return 0;


}
