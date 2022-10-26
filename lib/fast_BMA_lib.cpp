#include "EBMA_lib.h"

static bool Check_search_boundry(cv::Point &search_pt,int M_width,int M_height)
{
    if(search_pt.x < M_width && search_pt.x >= 0 && search_pt.y < M_height && search_pt.y >= 0)
        return true;
    else
        return false;
}
void Diamond_search(cv::Mat &current_frame,cv::Mat &ref_frame,cv::Point &block_pos,cv::Point &now_pos,cv::Point &mv_temp,double &min,short block_size,short step_size)
{
    int M_width = current_frame.cols;
    int M_height = current_frame.rows;
    cv::Point pre_mv_temp = mv_temp;
    mv_temp = cv::Point(0,0);
    for(int Sx = -1*step_size ; Sx <= 1*step_size ; Sx += step_size)
    {
        for(int Sy = -1*step_size ; Sy <= 1*step_size ; Sy += step_size)
        {
            if( (Sx != 0 && Sy != 0) || (Sx == (-pre_mv_temp.x) && Sy == (-pre_mv_temp.y))  )
                continue;
            cv::Point search_v = cv::Point(Sx,Sy);
            cv::Point search_pt(now_pos.x+Sx,now_pos.y+Sy);
            cv::Point now_mv(search_pt.x - block_pos.x,search_pt.y - block_pos.y);

            if(Check_search_boundry(search_pt,M_width,M_height))
            {
                double criterion = Cal_EDFD(current_frame,ref_frame,now_mv,block_pos,block_size);
                if(criterion < min)
                {
                    min = criterion;
                    mv_temp = search_v;
                }
            }
            else
            {
                mv_temp = cv::Point(0,0);
                goto here;
            }
        }
    }
    here:

    return;
}
void Square_search(cv::Mat &current_frame,cv::Mat &ref_frame,cv::Point &block_pos,cv::Point &now_pos,cv::Point &mv_temp,double &min,short block_size,short step_size)
{
    int M_width = current_frame.cols;
    int M_height = current_frame.rows;
    cv::Point pre_mv_temp = mv_temp;
    mv_temp = cv::Point(0,0);
    for(int Sx = -1*step_size ; Sx <= 1*step_size ; Sx += step_size)
    {
        for(int Sy = -1*step_size ; Sy <= 1*step_size ; Sy += step_size)
        {
            int position = std::abs(pre_mv_temp.x/step_size)+ std::abs(pre_mv_temp.x/step_size);
            if(position == 2) // corner
            {
                if( (Sx == (-pre_mv_temp.x) && Sy == (-pre_mv_temp.y)) || (Sx + pre_mv_temp.x + Sy + pre_mv_temp.y == step_size) )
                    continue;
            }
            else if(position == 1) //edge
            {
                if( !((Sx == (pre_mv_temp.x) && Sy == (pre_mv_temp.y)) || ((Sx - pre_mv_temp.x) + (Sy - pre_mv_temp.y) == std::abs(step_size) )))
                    continue;
            }

            cv::Point search_v = cv::Point(Sx,Sy);
            cv::Point search_pt(now_pos.x+Sx,now_pos.y+Sy);
            cv::Point now_mv(search_pt.x - block_pos.x,search_pt.y - block_pos.y);

            if(Check_search_boundry(search_pt,M_width,M_height))
            {
                double criterion = Cal_EDFD(current_frame,ref_frame,now_mv,block_pos,block_size);
                if(criterion < min  )
                {
                    min = criterion;
                    mv_temp = search_v;
                }
            }
        }
    }
}
void BMA_2Dlog(cv::Mat &current_frame,cv::Mat &ref_frame,cv::Mat &predict_frame,short search_size,short block_size)
{
    int M_width = current_frame.cols;
    int M_height = current_frame.rows;
    int candidate_cols = M_width/block_size;
    int candidate_rows = M_height/block_size;
    predict_frame = cv::Mat(current_frame.size(),current_frame.type());
    cv::Mat MV_line = cv::Mat::zeros(current_frame.size(),current_frame.type());
    cv::Mat MV_color = cv::Mat::zeros(current_frame.size(),CV_8UC3);
    short step_size = std::ceil(search_size/2);

    for(int j = 0; j < candidate_rows;j++)
    {
        for(int i = 0;i < candidate_cols;i++)
        {
            cv::Point MV(0,0);

            cv::Point block_pos(i*block_size,j*block_size); // the left top corner position of the candidate block
            cv::Point now_pos = block_pos;                  // the moving search point with square or diamand search
            cv::Point mv_temp(-1,-1);                       // the temp best search point

            short temp_step_size = step_size;
            bool search_one_block = true;
            double min = 10000;
            while(search_one_block)
            {
                if(temp_step_size == 1)
                    Square_search(current_frame,ref_frame,block_pos,now_pos,mv_temp,min,block_size,temp_step_size);
                else
                    Diamond_search(current_frame,ref_frame,block_pos,now_pos,mv_temp,min,block_size,temp_step_size);

                if(mv_temp.x == 0 && mv_temp.y == 0)
                {
                    if(temp_step_size == 1)
                        search_one_block = false;
                    else
                        temp_step_size = std::ceil((double)temp_step_size/2);
                }
                else
                {
                    MV.x += mv_temp.x;
                    MV.y += mv_temp.y;
                    now_pos.x += mv_temp.x;
                    now_pos.y += mv_temp.y;
                }
            }

            cv::Point global_MV(block_pos.x+MV.x,block_pos.y+MV.y);
            cv::line(MV_line,block_pos,global_MV,cv::Scalar(255));
            Set_MV_color(MV_color,MV,block_pos,block_size);
            Predict_ref_frame(predict_frame,ref_frame,MV,block_pos,block_size);
        }
    }
    cv::imshow("mv line",MV_line);
    cv::imshow("mv color",MV_color);
}

