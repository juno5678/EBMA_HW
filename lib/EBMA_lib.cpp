#include "EBMA_lib.h"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <cmath>

void Set_MV_color(cv::Mat &MV,cv::Mat &colorSpace,cv::Point &mv,cv::Point &set_pos,short  block_size)
{
    cv::Point center(colorSpace.cols/2,colorSpace.rows/2);
    for(int i = 0 ; i < block_size;i++)
    {
        for(int j = 0 ; j < block_size;j++)
        {
            MV.at<cv::Vec3b>(set_pos.y+j,set_pos.x+i)[0] = colorSpace.at<cv::Vec3b>(std::round(center.y+mv.y),std::round(center.x+mv.x))[0];
            MV.at<cv::Vec3b>(set_pos.y+j,set_pos.x+i)[1] = colorSpace.at<cv::Vec3b>(center.y+mv.y,center.x+mv.x)[1];
            MV.at<cv::Vec3b>(set_pos.y+j,set_pos.x+i)[2] = colorSpace.at<cv::Vec3b>(center.y+mv.y,center.x+mv.x)[2];
        }
    }

}

int Check_boundry(int now_pos,int search_size,int boundry)
{
    if(boundry == 0)
    {
        if(now_pos+search_size < boundry)
            return -now_pos;
    }
    else if(boundry >0 )
    {
        if(now_pos+search_size > boundry)
            return boundry-now_pos;
    }
    return search_size;
}
int Check_index_boundry(int index,int upper_boundry,int low_boundry)
{
    if(index >= upper_boundry)
        return upper_boundry-1;
    else if(index <= low_boundry)
        return low_boundry;
    else
        return index;
}
void EMBA(cv::Mat &current_frame,cv::Mat &ref_frame,cv::Mat &predict,short search_size,short block_size)
{
    int M_width = current_frame.cols;
    int M_height = current_frame.rows;
    int candidate_cols = M_width/block_size;
    int candidate_rows = M_height/block_size;
    predict = cv::Mat(current_frame.size(),current_frame.type());
    cv::Mat MV_line = cv::Mat::zeros(current_frame.size(),current_frame.type());
    cv::Mat MV_color = cv::Mat::zeros(current_frame.size(),CV_8UC3);
    cv::Mat colorSpace = cv::imread("../data/color_space.png");

    for(int j = 0; j < candidate_rows;j++)
    {
        for(int i = 0;i < candidate_cols;i++)
        {
            int min = 10000;
            cv::Point MV(0,0);
            for(int x = Check_boundry(i*block_size,-search_size,0) ; x < Check_boundry(i*block_size,search_size,M_width-1);x++)
            {
                for(int y = Check_boundry(j*block_size,-search_size,0) ; y < Check_boundry(j*block_size,search_size,M_height-1);y++)
                {
                    int sum = 0;
                    for(int bx = 0 ; bx < block_size;bx++)
                    {
                        for(int by = 0 ; by < block_size;by++)
                        {
                            cv::Point now_pos(i*block_size+bx,j*block_size+by);
                            cv::Point search_pos(i*block_size+bx+x,j*block_size+by+y);
                            int cur_I = current_frame.at<uchar>(now_pos);
                            int dx = current_frame.at<uchar>(now_pos.y,std::min(now_pos.x + 1,M_width-1 )) - cur_I;
                            int dy = current_frame.at<uchar>(std::min(now_pos.y+1,M_height-1),now_pos.x  )  - cur_I;
                            cv::Point v(x,y);
                            int ref_I = ref_frame.at<uchar>(now_pos);
                            int del_I = dx*v.x + dy*v.y;

                            int OF_value = std::sqrt((del_I + ref_I -cur_I) *(del_I + ref_I -cur_I));
                            sum += OF_value;
                        }
                    }
                    if(sum < min)
                    {
                        min = sum;
                        MV.x = x;
                        MV.y = y;
                    }
                }
            }
            cv::Point now_block_pos(i*block_size,j*block_size);
            cv::Point global_MV(now_block_pos.x+MV.x,now_block_pos.y+MV.y);
            cv::line(MV_line,now_block_pos,global_MV,cv::Scalar(255));
            Set_MV_color(MV_color,colorSpace,MV,now_block_pos,block_size);

            //if(MV.x > 0 || MV.y > 0)
            //{
            //    int arrow_lenth = 2;
            //    double mv_length = std::sqrt(std::pow(MV.x,2)+std::pow(MV.y,2));
            //    int arrow_pt1 = std::round(std::sqrt(2)/2 *((double)(-MV.x+MV.y)/(double)mv_length)*arrow_lenth);
            //    int arrow_pt2 = std::round(std::sqrt(2)/2 *((double)(-MV.x-MV.y)/(double)mv_length)*arrow_lenth);
            //    printf("arrow pt1 : %d , arrow pt2 : %d \n",arrow_pt1,arrow_pt2);

            //    cv::Point arrow1(arrow_pt1,arrow_pt2);
            //    cv::Point arrow2(arrow_pt2,arrow_pt1);

            //    cv::line(Motion_vector,global_MV,cv::Point(global_MV.x+arrow1.x,global_MV.y+arrow1.y),cv::Scalar(255));
            //    cv::line(Motion_vector,global_MV,cv::Point(global_MV.x+arrow2.x,global_MV.y+arrow2.y),cv::Scalar(255));

            //}

            for(int bx = 0 ; bx < block_size;bx++)
            {
                for(int by = 0 ; by<block_size;by++)
                {
                    //printf("width : %d , height : %d \n",M_width,M_height);
                    int predict_index_x = Check_index_boundry(i*block_size+bx+MV.x,M_width,0);
                    int predict_index_y = Check_index_boundry(j*block_size+by+MV.y,M_height,0);
                    predict.at<uchar>(j*block_size+by,i*block_size+bx) = ref_frame.at<uchar>(predict_index_y,predict_index_x);
                }
            }

        }
    }
    cv::imshow("color space",colorSpace);
    cv::imshow("mv line",MV_line);
    cv::imshow("mv color",MV_color);
}
