#include "EBMA_lib.h"

int main(int argc,char** argv)
{

    std::string images_path = std::string(argv[1]);
    std::vector<std::string> filenames_in_folder;
    cv::String images_path_cv = images_path;
    std::vector<cv::String> filenames_in_folder_cv;
    //取得照片
    cv::glob(images_path_cv, filenames_in_folder_cv);
    for (auto &i : filenames_in_folder_cv)
        filenames_in_folder.push_back(i);


    std::vector<std::string> jpg_filenames_path;
    std::vector<std::string> jpg_filenames;
    std::vector<std::string> jpg_filenames_without_ext;

    //確認檔名
    for(auto &i : filenames_in_folder)
    {
            int pos_filename = 0;
            if ((1 + i.find_last_of("\\")) < i.length()) pos_filename = 1 + i.find_last_of("\\");
            if ((1 + i.find_last_of("/")) < i.length()) pos_filename = std::max(pos_filename, 1 + (int)i.find_last_of("/"));

            std::string const filename = i.substr(pos_filename);
            std::string const ext = i.substr(i.find_last_of(".") + 1);
            std::string const filename_without_ext = filename.substr(0, filename.find_last_of("."));

            if (ext == "png")
            {
                jpg_filenames_without_ext.push_back(filename_without_ext);
                jpg_filenames.push_back(filename);
                jpg_filenames_path.push_back(i);
            }
    }

    std::sort(jpg_filenames.begin(), jpg_filenames.end());
    std::sort(jpg_filenames_path.begin(), jpg_filenames_path.end());

    if (jpg_filenames.size() == 0) {
        std::cout << "Error: Image files not found by path: " << images_path << std::endl;
        return 0;
    }
    std::cout << "find " << jpg_filenames.size() << "of plate in this folder" << std::endl;

    std::vector<cv::Mat> images_array;
    for(size_t i  = 0; i < jpg_filenames.size(); i ++)
    {
        cv::Mat src = imread(jpg_filenames_path[i],cv::IMREAD_COLOR);
    images_array.push_back(src);
    cv::waitKey(1);
    cv::imshow("src",src);
    cv::waitKey(0);
    //Mat result;
    //string place_path = "../../align_license_plate/aligned_image_"+jpg_filenames[i];
    //imwrite(place_path,result);
    }

    cv::destroyAllWindows();

    return 0 ;


}

