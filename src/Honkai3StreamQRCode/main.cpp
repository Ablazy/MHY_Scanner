﻿//reference
//https://github.com/ikexing-cn/bilibili-live-stream
//https://github.com/HonkaiScanner/scannerHelper
//https://www.bilibili.com/video/BV1TP411p7cC
//https://github.com/yespon/yazi
#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <opencv2/opencv.hpp>
#include <zbar.h>
#include "..//Scan/Scan.h"
#include <Windows.h>
#include <future>

extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/imgutils.h>
#include <libavutil/time.h>
#include <libswscale/swscale.h>
}

#include "v2api.h"
#include "Download.h"

void scanMain(std::promise<std::string> url)
{
	Sleep(1500);
	Scan scan;
	scan.OpenVideo("..\\Honkai3StreamQRCode\\cache\\output.flv");
	int index = scan.GetStreamIndex(AVMEDIA_TYPE_VIDEO);
	int frameCount = 0;
	scan.FFmpegDecoder(index);
	scan.OpenDecoder(index);
	scan.buffer(scan.pFrameBGR);
	scan.swsctx(&scan.swsCtx);
	std::string qrCode;

	scan.opencvinit();

	int64_t latestTimestamp = av_gettime_relative();

	if (scan.avformatContext->streams[index]->start_time != AV_NOPTS_VALUE)
	{
		int64_t streamTimestamp = av_rescale_q(scan.avformatContext->streams[index]->start_time,
			scan.avformatContext->streams[index]->time_base, { 1, AV_TIME_BASE });
		if (streamTimestamp > latestTimestamp)
		{
			latestTimestamp = streamTimestamp;
		}
	}

	//av_seek_frame(scan.avformatContext, index, 10 * AV_TIME_BASE, 1);
	while (/*scan.read(scan.avPacket) >= 0*/true)
	{
		scan.read(scan.avPacket);
		if (scan.avPacket->stream_index == index)
		{
			avcodec_send_packet(scan.avCodecContext, scan.avPacket);
			while (scan.ReceiveFrame(scan.avframe) == 0)
			{
				av_seek_frame(scan.avformatContext, -1, latestTimestamp, AVSEEK_FLAG_BACKWARD);
				// 转换像素格式
				sws_scale(scan.swsCtx, scan.avframe->data, scan.avframe->linesize, 0,
					scan.avCodecContext->height, scan.pFrameBGR->data, scan.pFrameBGR->linesize);

				if (frameCount % 15 == 0)
				{

					// 显示视频帧
					cv::Mat img(scan.avCodecContext->height, scan.avCodecContext->width, CV_8UC3, scan.pFrameBGR->data[0]);

					cv::Mat gray;
					cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);
					zbar::Image image(gray.cols, gray.rows, "Y800", gray.data, gray.cols * gray.rows);
					scan.scanner.scan(image);
					// 遍历扫描结果，输出二维码信息
					for (zbar::Image::SymbolIterator symbol = image.symbol_begin(); symbol != image.symbol_end(); ++symbol)
					{
						std::cout << "Decoded: " << symbol->get_type_name() << std::endl;
						std::cout << symbol->get_data() << std::endl;
						qrCode = symbol->get_data();
					}
					image.set_data(nullptr, 0);
					imshow("Video", img);
					if (cv::waitKey(1) == 'q')
					{
						break;
					}
				}
				frameCount++;
			}
			if (qrCode.find("biz_key=bh3_cn") != std::string::npos)
			{
				url.set_value(qrCode);
				break;
			}
			if (qrCode != "")
			{
				std::cout << "非崩坏3三二维码" << std::endl;
			}
		}
		av_packet_unref(scan.avPacket);
	}
}
int main(int argc, char* argv[])
{
	v2api v2api;
	std::string playurl = v2api.Initialize();
	std::string qrCode;


	Download down;
	std::thread th([&down, playurl]() 
	{
		down.curlDownlod(playurl);
	}); 
	
	std::promise<std::string> QRcodeUrl;
	std::future<std::string> future_result = QRcodeUrl.get_future();
	std::thread th1(scanMain, std::move(QRcodeUrl));
	th1.join();
	qrCode = future_result.get();
	std::cout << "========================" << std::endl;
	std::cout << qrCode << std::endl;
	//down.getstop();
	if(qrCode !="")
	{
		down.stopDownloadAfterDelay();
		th.join();
	}


	//getchar();
	
	//std::ifstream fin("./config.json");
	//std::stringstream ss;
	//ss << fin.rdbuf();
	//const std::string& str = ss.str();
	//json::Json v;
	//v.parse(str);
	//json::Json a;
	//a = v["quality_description"][0];
	//std::cout << a.str() <<std::endl;
	////！！！！！v.clear();
	std::cout << "Exit" << std::endl;
	return 0;
}