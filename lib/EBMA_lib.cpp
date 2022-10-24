#include "EBMA_lib.h"
#include <cmath>

cv::Point Cal_MV(cv::Point &pt1,cv::Point &pt2)
{
    return cv::Point(pt1.x-pt2.x,pt1.y-pt2.y);
}
int Check_boundry(int now_pos,int search_size,int boundry)
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

    for(int j = 0; j < candidate_rows;j++)
    {
        for(int i = 0;i < candidate_cols;i++)
        {
            int min = 10000;
            cv::Point MV(0,0);
            for(int x = Check_boundry(i*block_size,-search_size,0) ; x < Check_boundry(i*block_size,search_size,M_width);x++)
            {
                for(int y = Check_boundry(j*block_size,-search_size,0) ; y < Check_boundry(j*block_size,search_size,M_height);y++)
                {
                    int sum = 0;
                    for(int bx = 0 ; bx < block_size;bx++)
                    {
                        for(int by = 0 ; by < block_size;by++)
                        {
                            cv::Point now_pos(i*block_size+bx,j*block_size+by);
                            cv::Point search_pos(i*block_size+x,j*block_size+y);
                            if(now_pos.x >= M_width || now_pos.x < 0)
                                printf("now i : %d , bx : %d , mv.x :%d , index_x : %d \t ",i,bx,MV.x,i*block_size+x);
                            if(now_pos.y >= M_height || now_pos.y < 0)
                                printf("now j : %d , by : %d , mv.y :%d , index_y : %d \n ",j,by,MV.y,j*block_size+y);
                            if(search_pos.x >= M_width || search_pos.x < 0)
                                printf("i : %d , bx : %d , mv.x :%d , index_x : %d \t ",i,bx,MV.x,i*block_size+x);
                            if(search_pos.y >= M_height || search_pos.y < 0)
                                printf("j : %d , by : %d , mv.y :%d , index_y : %d \n ",j,by,MV.y,j*block_size+y);
                            int cur_I = current_frame.at<uchar>(now_pos);
                            int dx = current_frame.at<uchar>(now_pos.y,std::min(now_pos.x + 1,M_width-1 )) - cur_I;
                            int dy = current_frame.at<uchar>(std::min(now_pos.y+1,M_height-1),now_pos.x  )  - cur_I;
                            cv::Point v = Cal_MV(search_pos,now_pos);
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
            for(int bx = 0 ; bx < block_size;bx++)
            {
                for(int by = 0 ; by<block_size;by++)
                {
                    //printf("width : %d , height : %d \n",M_width,M_height);
                    int predict_index_x = Check_index_boundry(i*block_size+bx+MV.x,M_width,0);
                    int predict_index_y = Check_index_boundry(j*block_size+by+MV.y,M_height,0);
                    //printf("predict_index_x : %d , predict_index_y : %d \n",predict_index_x,predict_index_y);
                    //if(predict_index_x >= M_width || predict_index_x < 0)
                    //    printf("predict_index_x : %d , predict_index_y : %d \n",predict_index_x,predict_index_y);
                    //if(predict_index_y >= M_height || predict_index_y < 0)
                    //    printf("predict_index_x : %d , predict_index_y : %d \n",predict_index_x,predict_index_y);
                    //if(i*block_size+bx >= M_width || i*block_size+bx < 0)
                    //    printf("i : %d , bx : %d  , index_x : %d \t ",i,bx,i*block_size+bx);
                    //if(j*block_size+by >= M_height || j*block_size+by < 0)
                    //    printf("j : %d , by : %d  , index_y : %d \n ",j,by,j*block_size+by);
                    predict.at<uchar>(j*block_size+by,i*block_size+bx) = ref_frame.at<uchar>(predict_index_y,predict_index_x);
                }
            }

        }
    }
}
