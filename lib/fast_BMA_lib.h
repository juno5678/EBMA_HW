#ifndef __FAST_BMA_LIB_H__
#define __FAST_BMA_LIB_H__

#include "EBMA_lib.h"
void BMA_2Dlog(cv::Mat &current_frame,cv::Mat &ref_frame,cv::Mat &predict_frame,short search_size,short block_size);
void Diamond_search(cv::Mat &current_frame,cv::Mat &ref_frame,cv::Point &block_pos,cv::Point &now_pos,cv::Point &mv_temp,double &min,short block_size,short step_size);
void Diamond_search(cv::Mat &current_frame,cv::Mat &ref_frame,cv::Point &block_pos,cv::Point &now_pos,cv::Point &mv_temp,double &min,short block_size,short step_size);
#endif
