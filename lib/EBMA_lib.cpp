#include "EBMA_lib.h"
#include <cmath>

double Cal_dis(cv::Point &pt1,cv::Point &pt2)
{
    return std::sqrt((pt1.x-pt2.x)*(pt1.x-pt2.x)+(pt1.y-pt2.y)*(pt1.y-pt2.y));
}
short Check_boundry(short now_pos,short search_size,short boundry)
{
    if(boundry == 0)
    {
        if(now_pos+search_size < boundry)
            return now_pos-boundry;
    }
    else if(boundry >0 )
    {
        if(now_pos+search_size > boundry)
            return boundry-now_pos;
    }
    return search_size;
}
void EMBA(cv::Mat &current_frame,cv::Mat &ref_frame,cv::Mat &predict,short search_size,short block_size)
{
    short M_width = current_frame.cols;
    short M_height = current_frame.rows;
    short candidate_cols = M_width/block_size;
    short candidate_rows = M_height/block_size;

    for(int j = 0; j < candidate_rows,j++)
    {
        for(int i = 0;i < candidate_cols;i++)
        {
            for(int x = Check_boundry(j*block_size,-search_size,0) ; x < Check_boundry(j*block_size,search_size,M_width);x++)
            {
                for(int y = Check_boundry(i*block_size,-search_size,0) ; y < Check_boundry(i*block_size,search_size,M_height);y++)
                {
                    for(int bx = 0 ; bx < block_size;bx++)
                    {
                        for(int by = 0 ; by < block_size;by++)
                        {
                            short cur_I = current_frame.at<uchar>(j*block_size,i*block_size);
                            short ref_I = ref_frame.at<uchar>(j*block_size,i*block_size);
                            short del_I = current_frame.at<uchar>(j*block_size+x,i*block_size+y) - cur_I;

                            int OF_value = std::sqrt(std::pow(del_I))

                        }
                    }
                }
            }
        }
    }

}
