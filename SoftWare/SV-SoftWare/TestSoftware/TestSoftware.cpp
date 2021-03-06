// TestSoftware.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "..\SV-SoftWare\SV-SoftWare.h"

#pragma comment(lib, "SV-SoftWare.lib")

int main()
{
	std::list<Service_Struct> data = {};
	SV_ASSIST::Software::PowerManagement();
	SV_ASSIST::Software::EnumService(data);
	std::list<TaskScheduler_Struct> data2 = {};
	std::list<TaskScheduler_Struct> data3 = {};
	std::vector<RegKeyDetail> data4 = {};
	SV_ASSIST::Software::EnumFolderTaskScheduler(data2);
	SV_ASSIST::Software::EnumRuningTaskScheduler(data3);
	SV_ASSIST::Software::AutoList(data4);
	std::vector<ConnInfo> data44 = {};
	std::vector<ConnInfo> data45 = {};
	SV_ASSIST::Software::GetTcpConnect(data44);
	SV_ASSIST::Software::GetUdpConnect(data45);
    return 0;
}

