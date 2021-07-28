//
// Created by root on 9/17/17.
//

#include <stdio.h>

#include "depth_to_color.h"
#include "flash_bin_parser.h"
#include <opencv2/opencv.hpp>


int main(int argc, char *argv [])
{
    DepthToColor d2c;
    printf("Version: %s\n", d2c.GetVersion());

    if (4 != argc)
    {
        fprintf(stderr, "Usage: %s color_img depth_img camera_param\n", argv[0]);
        return -1;
    }

    const char *color_img_fname = argv[1];
    const char *depth_img_fname = argv[2];
    const char *camera_param = argv[3];

#ifdef LOAD_INI_FILE
	bool success = d2c.LoadParameters(camera_param);
#else	// load SW_D2C from binary file/buffer

	Flash_bin_parser flashParser;

	FILE* fp = fopen("./flash.bin", "rb");

	printf("buffer size = %d \n", flashParser.getDataSize());

	char* buffer = new char[flashParser.getDataSize()];

	fread(buffer, 1, flashParser.getDataSize(), fp);

	fclose(fp);

	SW_D2C soft_d2c;

	flashParser.parse_buffer_to_params(buffer, &soft_d2c);
	bool success = d2c.LoadParameters(soft_d2c);
	delete [] buffer;
#endif
    if (!success)
    {
        fprintf(stderr, "LoadParameters failed!\n");
        return -1;
    }

    cv::Mat color_img = cv::imread(color_img_fname, cv::IMREAD_COLOR);
    cv::cvtColor(color_img, color_img, CV_BGR2RGB);
    cv::Mat depth_img = cv::imread(depth_img_fname, cv::IMREAD_UNCHANGED);

    int depth_width = depth_img.cols;
    int depth_height = depth_img.rows;
    int color_width = color_img.cols;
    int color_height = color_img.rows;

    int size = color_width * color_height * sizeof(uint16_t);
    uint16_t * aligned_depth = new uint16_t[size];

    success = d2c.PrepareDepthResolution(depth_width, depth_height);
    if (!success)
    {
        fprintf(stderr, "PrepareDepthResolution failed!\n");
        return -1;
    }

    // 关闭畸变
    success = d2c.EnableDistortion(false);
    if (!success)
    {
        fprintf(stderr, "EnableDistortion failed!\n");
        return -1;
    }
    // fill gaps
	success = d2c.EnableDepthGapFill(true);
	if (!success)
	{
		fprintf(stderr, "EnableDepthGapFill failed!\n");
		return -1;
	}

    // 每个深度单位=1.0 mm
    success = d2c.SetDepthUnit(1);
    if (!success)
    {
        fprintf(stderr, "EnableDistortion failed!\n");
        return -1;
    }

    // 设置原始深度图中，感兴趣的深度范围
    success = d2c.SetDepthRange(300, 900);
    if (!success)
    {
        fprintf(stderr, "EnableDistortion failed!\n");
        return -1;
    }

    int ret = d2c.D2C(depth_img.ptr<uint16_t>(), depth_width, depth_height,
                      aligned_depth, color_width, color_height);
    if (ret)
    {
        fprintf(stderr, "D2C failed!\n");
        delete [] aligned_depth;
        aligned_depth = nullptr;
        return -1;
    }

    delete [] aligned_depth;
    aligned_depth = nullptr;

    printf("D2C success!\n");

    return 0;
}