cmake_minimum_required(VERSION 3.26)

add_library(MHY_Scanner_Core STATIC 
	BiliGameApi.cpp
	CryptoKit.cpp
	HttpClient.cpp
	LiveStreamLink.cpp
	Mihoyosdk.cpp
	OfficialApi.cpp
	QRScanner.cpp
	ScreenScan.cpp
	ConfigDate.cpp
	#VideoProcessor.cpp
)