#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <string.h>
#include <iostream>
#include "EBMA_lib.h"
#include "fast_BMA_lib.h"

int Sum(cv::Mat &frame)
{
    int sum = 0;
    cv::Mat sum_frame = frame.clone();
    for(int i = 0 ;i < frame.rows;i++)
    {
        //printf(" %d ",sum_frame.at<cv::Vec3b>(0,i)[0]);
        for(int j = 0 ; j < frame.cols;j++)
        {
            int temp = sum_frame.at<cv::Vec3b>(j,i)[0]+sum_frame.at<cv::Vec3b>(j,i)[1]+sum_frame.at<cv::Vec3b>(j,i)[2];

            sum += temp;
        }
    }
    printf("\n");
    return sum;
}
void cal_diff(cv::Mat &fst_frame,cv::Mat &sec_frame,cv::Mat &diff_frame)
{
    diff_frame = cv::Mat(fst_frame.size(),fst_frame.type());
    for(int i = 0 ;i < fst_frame.rows;i++)
    {
        for(int j = 0 ; j < fst_frame.cols;j++)
        {
            diff_frame.at<cv::Vec3b>(j,i)[0] = std::abs(fst_frame.at<cv::Vec3b>(j,i)[0]-sec_frame.at<cv::Vec3b>(j,i)[0]);
            diff_frame.at<cv::Vec3b>(j,i)[1] = std::abs(fst_frame.at<cv::Vec3b>(j,i)[1]-sec_frame.at<cv::Vec3b>(j,i)[1]);
            diff_frame.at<cv::Vec3b>(j,i)[2] = std::abs(fst_frame.at<cv::Vec3b>(j,i)[2]-sec_frame.at<cv::Vec3b>(j,i)[2]);
        }
    }

}

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
    cv::Mat l_cur_frame = L_cur_frame.clone();
    printf("fst mse with cur and ref : %f\n",Cal_MSE(L_cur_frame,l_cur_frame));
    double sum = 0;
    double MSE;
    for(int i = 0 ; i < predict_frame.rows;i++)
    {
        for(int j = 0 ; j < predict_frame.cols;j++)
        {
            int predict_I = L_cur_frame.at<uchar>(j,i);
            int target_I = l_cur_frame.at<uchar>(j,i);
            if(predict_I != target_I)
            {
                printf("predict  (%d,%d):%d\n",i,j,predict_I);
                printf("target  (%d,%d):%d\n",i,j,target_I);
            }
            MSE = std::round(std::sqrt(std::pow(predict_I - target_I,2))*1000)/1000;
            sum +=MSE;

        }
    }
    printf("mse sum : %3f\n",sum);
    Get_chrominance(ref_img,L_ref_frame);
    cv::imshow("cur img",cur_img);
    cv::imshow("ref img",ref_img);

    //cal_diff(cur_frame,cur_frame,diff_frame);
    //cv::imshow("diff frame",diff_frame);

    //printf("fst current frame sum : %d\n",Sum(cur_img));
    //printf("fst ref frame sum : %d\n",Sum(ref_img));


    //EBMA(L_cur_frame,L_ref_frame,predict_frame,6,4);
    //printf("sec mse with cur and ref : %f\n",Cal_MSE(L_cur_frame,L_cur_frame));

    cv::imshow("current frame",L_cur_frame);
    cv::imshow("ref frame",L_ref_frame);
    //cv::imshow("predict frame",predict_frame);
    cv::waitKey(0);
    return 0;


}
