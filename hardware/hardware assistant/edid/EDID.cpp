// EDID.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include <fstream>
#include "EDID.h"
#include "EDID src.h"
#include <boost/algorithm/string.hpp>
using namespace std;

union EDIDManufacturer
{
	struct
	{
		WORD _Third : 5;
		WORD _Second : 5;
		WORD _First : 5;
		WORD Reserved : 1;
	}bits;
	WORD __Manufacturer;
};

namespace SV_ASSIST
{
	namespace Display
	{
		class CEDID {
		public:
			static _EDID_INFO EDIDData;
		};

		_EDID_INFO CEDID::EDIDData;


		const EDID& GetEDID(const UINT Num)
		{
			return CEDID::EDIDData.ReturnEDID(Num);
		}

		void Exec()
		{
			TCHAR   szCurDir[MAX_PATH];
			if (GetModuleFileName(NULL, szCurDir, MAX_PATH) == 0) {

				printf("GetCurrentDirectory failed!  Error = %d \n", GetLastError());
				return;

			}
			(_tcsrchr(szCurDir, _T('\\')))[1] = 0; // 删除文件名，只获得路径字串
			wstring dir(szCurDir);
			dir.append(L"\\HardWare\\EDID\\");
			if (_waccess(dir.c_str(), 0) == -1)
			{
				OutputDebugString(dir.c_str());
				OutputDebugString(L" is not existing;\n now make it");
				for (int i = 0; i < dir.size(); i++)
				{
					if (dir[i] == '\\')
					{
						dir[i] = '\0';
						if (_waccess(dir.c_str(), 0) != 0)
						{
							int flag = _wmkdir(dir.c_str());
							if (flag == 0)
							{
								OutputDebugString(L"make successfully");
							}
							else {
								OutputDebugString(L"make errorly");
								return;
							}
						}
						dir[i] = '\\';
					}
				}
			}
			CEDID::EDIDData.UpdateData();
			UINT Num = GetEDIDNum();
			for(DWORD a = 0; a < Num; a++)
			{
				const EDID edid= GetEDID(a);
				wofstream outfile;
				outfile.open(dir + _T("\\edid") + to_wstring(a + 1) + _T(".txt"));
				
				EDIDManufacturer  buf;
				buf.__Manufacturer = edid.Manufacturer.Manufacturer;
				buf.__Manufacturer = ((buf.__Manufacturer & 0xFF) << 8) | ((buf.__Manufacturer & 0xFF00) >> 8);
				char temp[4];

				temp[0] = buf.bits._First + 'A' - 1;
				temp[1] = buf.bits._Second +'A' - 1;
				temp[2] = buf.bits._Third + 'A' - 1;
				temp[3] = 0;
				outfile << _T("EDID ( Extended Display Identification Data) Report #") << to_wstring(a + 1) << endl;
				outfile << _T("	Vendor / Product Identification") << endl;
				outfile << _T("            Manufacture:                            ") << temp << endl;
				outfile << _T("            Monitor Serial Number :                 ") << edid.SerialNumber << endl;
				char aaa[16] = {};
				sprintf_s(aaa, "%08X", edid.Productcode);
				outfile << _T("            Product Code:                           ") << aaa << endl;
				outfile << _T("            Week/Year:                              ") << edid.Week << _T("/") << edid.Year + 1990 << endl;
				outfile << _T("            Version:                                ") << edid.EDIDVersion[0] << _T(".") << edid.EDIDVersion[1] << endl;
				outfile << _T("    Video input parameters bitmap") << endl;
				if(edid.VideoInputParameters & 0x80)
				{
					outfile << _T("            Digital input") << endl;
						short bitdepth = (edid.VideoInputParameters & 0x70) >> 4;
						if (bitdepth == 0 || bitdepth == 0x111)
							outfile << _T("            Bit depth:                              Undefined") << endl;
						else
							outfile << _T("            Bit depth:                              %1") << (bitdepth - 1) * 2 + 6 << endl;
						short videointerface = edid.VideoInputParameters & 0xF;
					switch (videointerface)
					{
					case 1:
						outfile << _T("            Video interface:                        HDMIa") << endl;
						break;
					case 2:
						outfile << _T("            Video interface:                        HDMIb") << endl;
						break;
					case 4:
						outfile << _T("            Video interface:                        MDDI") << endl;
						break;
					case 5:
						outfile << _T("            Video interface:                        DisplayPort") << endl;
						break;
					default:
						outfile << _T("            Video interface:                        Undefined") << endl;
						break;
					}
				}
				else
				{
					outfile << _T("            Analog input") << endl;

				}
				outfile << _T("            Max Horizontal Image Size:              ") << (edid.HorizontalScreenSize) << _T("cn") << endl;
				outfile << _T("            Max Vertical Image Size:                ") << (edid.VerticalScreenSize) << _T("cn") << endl;
				outfile << _T("            Gamma:                                  ") << (((edid.DisplayGamma + 100) / 100.0)) << endl;
				outfile << _T("    FeatureSupport:") << endl;
				outfile << _T("            Standby:                                ") << (edid.Features & 0x80 ? _T("Yes") : _T("No")) << endl;
				outfile << _T("            Suspend:                                ") << (edid.Features & 0x40 ? _T("Yes") : _T("No")) << endl;
				outfile << _T("            Active off:                             ") << (edid.Features & 0x20 ? _T("Yes") : _T("No")) << endl;
			//    outfile << _T("            standby:                                ") + (edid.Features & 0x80 ? _T("Yes") : _T("No"))<< endl;
			//    outfile << _T("            Color space:                            ") + (edid.Features & 0x80 ? _T("Yes") : _T("No"))<< endl;
				outfile << _T("            standby:                                ") << (edid.Features & 0x04 ? _T("Yes") : _T("No")) << endl;
				outfile << _T("            Preferred timing mode specified:        ") << (edid.Features & 0x02 ? _T("Yes") : _T("No")) << endl;
				outfile << _T("            Continuous timings with GTF or CVT:     ") << (edid.Features & 0x01 ? _T("Yes") : _T("No")) << endl;
				outfile << _T("    Color Characteristic:") << endl;
				double Red_x = ((edid.RedxValue << 2 )| ((edid.RedAndGreen & 0xC0 ) >> 6 ));
				double Red_y = ((edid.RedyValue<< 2) | ((edid.RedAndGreen & 0x30 ) >> 4));
				double Green_x = ((edid.GreenxyValue[0] << 2 )| ((edid.RedAndGreen & 0x0C ) >> 2));
				double Green_y = ((edid.GreenxyValue[1] << 2)| ((edid.RedAndGreen & 0x03 ) >> 0));
				double Blue_x = ((edid.BluexyValue[0] << 2) | ((edid.BlueAndWhite & 0xC0 ) >> 6));
				double Blue_y = ((edid.BluexyValue[1] << 2 )| ((edid.BlueAndWhite & 0x30 ) >> 4));
				double White_x = ((edid.WhitexyValue[0] << 2 )| ((edid.BlueAndWhite & 0x0C ) >> 2));
				double White_y = ((edid.WhitexyValue[1] << 2 )| ((edid.BlueAndWhite & 0x03 ) >> 0));
				outfile << _T("            Red:                              X = ") << (Red_x / 1024) << "	Y = "<< (Red_y / 1024, 'f', 3) << endl;
				outfile << _T("            Red:                              X = ") << (Green_x / 1024) << "	Y = " << (Green_y / 1024, 'f', 3) << endl;
				outfile << _T("            Red:                              X = ") << (Blue_x / 1024) << "	Y = " << (Blue_y / 1024, 'f', 3) << endl;
				outfile << _T("            Red:                              X = ") << (White_x / 1024) << "	Y = " << (White_y / 1024, 'f', 3) << endl;
				//...
				outfile << _T("    Established Timings:") << endl;
				if(edid.EstablishedTiming[0] & 0x80)
				outfile << _T("            720x400 @ 70 Hz ") << endl;
				if(edid.EstablishedTiming[0] & 0x40)
				outfile << _T("            720x400 @ 88 Hz ") << endl;
				if(edid.EstablishedTiming[0] & 0x20)
				outfile << _T("            640x480 @ 60 Hz ") << endl;
				if(edid.EstablishedTiming[0] & 0x10)
				outfile << _T("            640x480 @ 67 Hz ") << endl;
				if(edid.EstablishedTiming[0] & 0x08)
				outfile << _T("            640x480 @ 72 Hz ") << endl;
				if(edid.EstablishedTiming[0] & 0x04)
				outfile << _T("            640x480 @ 75 Hz ") << endl;
				if(edid.EstablishedTiming[0] & 0x02)
				outfile << _T("            800x600 @ 56 Hz ") << endl;
				if(edid.EstablishedTiming[0] & 0x01)
				outfile << _T("            800x600 @ 60 Hz ") << endl;

				if(edid.EstablishedTiming[1] & 0x80)
				outfile << _T("            800x600 @ 72 Hz ") << endl;
				if(edid.EstablishedTiming[1] & 0x40)
				outfile << _T("            800x600 @ 75 Hz ") << endl;
				if(edid.EstablishedTiming[1] & 0x20)
				outfile << _T("            832x624 @ 75 Hz ") << endl;
				if(edid.EstablishedTiming[1] & 0x10)
				outfile << _T("            1024x768 @ 87 Hz ") << endl;
				if(edid.EstablishedTiming[1] & 0x08)
				outfile << _T("            1024x768 @ 60 Hz ") << endl;
				if(edid.EstablishedTiming[1] & 0x04)
				outfile << _T("            1024x768 @ 72 Hz ") << endl;
				if(edid.EstablishedTiming[1] & 0x02)
				outfile << _T("            1024x768 @ 75 Hz ") << endl;
				if(edid.EstablishedTiming[1] & 0x01)
				outfile << _T("            1280x1024 @ 75 Hz ") << endl;

				if(edid.EstablishedTiming[2] & 0x80)
				outfile << _T("            1152x870 @ 75Hz ") << endl;

				for(int i = 0; i < 16; i+=2)
				{
					if(edid.StandardTiming[i] == 0 || (edid.StandardTiming[i] == 1 && 1 == edid.StandardTiming[i + 1]))
					{
						outfile << _T("    Standard Timing #") << (i / 2) << endl;
				
						outfile << _T("          Unused") << endl;
					}
					else
					{
						outfile << _T("    Standard Timing #") << (i / 2) << endl;
						int horizontal = (edid.StandardTiming[i] + 31) * 8;
						int RefreshRate  = (edid.StandardTiming[i + 1] & 0x3F) + 60;
						switch(((edid.StandardTiming[i]+1) & 0xC) >> 6)
						{
						case 0:
							outfile << _T("            ") << horizontal << _T(" x ") << (horizontal / 16 * 10) << _T(" @ ") << RefreshRate << _T("Hz") << endl;
							break;
						case 1:
							outfile << _T("            ") << horizontal << _T(" x ") << (horizontal / 4 * 3) << _T(" @ ") << RefreshRate << _T("Hz") << endl;
							break;
						case 2:
							outfile << _T("            ") << horizontal << _T(" x ") << (horizontal / 5 * 4) << _T(" @ ") << RefreshRate << _T("Hz") << endl;
							break;
						case 4:
							outfile << _T("            ") << horizontal << _T(" x ") << (horizontal / 16 * 9) << _T(" @ ") << RefreshRate << _T("Hz") << endl;
							break;
						}

						}

				}
				for(short i = 0; i < 4 ; i++)
				{
					EDID_Other_Monitor_Descriptors OtherDesc;
					int minvoffset,minhoffset, maxvoffset,  maxhoffset;
					memcpy(&OtherDesc, edid.Descriptor + i, sizeof(EDID_Other_Monitor_Descriptors));
					if(OtherDesc.Reserve == 0 && OtherDesc.Reserved[0] == 0 && OtherDesc.Reserved[1] == 0 && OtherDesc.Reserved[2] == 0)
					{
						wchar_t* wide = new wchar_t[15]{};
						MultiByteToWideChar(CP_UTF8, 0, (const char*)edid.Descriptor[i].Display_Serial_Number.Display_Serial_Number, -1, wide, 13);
						wstring t(wide);
						switch(OtherDesc.Descriptor_Type)
						{
						case 0xFF:
							outfile << _T("    Display serial number Descriptor:") << endl;
							boost::trim(t);
							outfile << _T("        Display serial number:                          ") << t << endl;
							break;
						case 0xFD:
							outfile << _T("    Display Range Limits Descriptor:") << endl;
							edid.Descriptor[i].Display_Range_Limits_Descriptor.Offsets_for_display_range_limits.Offsets_for_display_range_limits & 0x1 ? minvoffset = 0xFF : minvoffset = 0;
							edid.Descriptor[i].Display_Range_Limits_Descriptor.Offsets_for_display_range_limits.Offsets_for_display_range_limits & 0x2 ? minhoffset = 0xFF : minhoffset = 0;
							edid.Descriptor[i].Display_Range_Limits_Descriptor.Offsets_for_display_range_limits.Offsets_for_display_range_limits & 0x4 ? maxvoffset = 0xFF : maxvoffset = 0;
							edid.Descriptor[i].Display_Range_Limits_Descriptor.Offsets_for_display_range_limits.Offsets_for_display_range_limits & 0x8 ? maxhoffset = 0xFF : maxhoffset = 0;
							outfile << _T("        Min. Horizontal rate:                       ") << (edid.Descriptor[i].Display_Range_Limits_Descriptor.Minimum_horizontal_field_rate + minhoffset) << _T("khz") << endl;
							outfile << _T("        Max. Horizontal rate:                       ") << (edid.Descriptor[i].Display_Range_Limits_Descriptor.Maximum_horizontal_field_rate + maxhoffset) << _T("khz") << endl;
							outfile << _T("        Min. Vertical rate:                         ") << (edid.Descriptor[i].Display_Range_Limits_Descriptor.Minimum_vertical_field_rate + minvoffset) << _T("khz") << endl;
							outfile << _T("        Max. Vertical rate:                         ") << (edid.Descriptor[i].Display_Range_Limits_Descriptor.Maximum_vertical_field_rate + maxvoffset) << _T("khz") << endl;
							outfile << _T("        Max. Pixel Clock:                           ") << (edid.Descriptor[i].Display_Range_Limits_Descriptor.Maximum_pixel_clock_rate * 10) << _T("Mhz") << endl;

							break;
						case 0xFC:
							outfile << _T("    Display name Descriptor:") << endl;
							boost::trim(t);
							outfile << _T("        Display name number:                          ") << t << endl;
							break;
						default:
							outfile << _T("    Descriptors #") << (i) << endl;
							outfile << _T("        To be continued") << endl;
							break;
						}
						delete wide;
						wide = nullptr;
					}
					else
					{
						outfile << _T("    Detailed Timing Descriptor:") << endl;
						int temp = edid.Descriptor[i].DetailDesc.Pixel_Clock;
						//temp = ((temp & 0xFF) << 8) | ((temp & 0xFF00) >> 8);
						outfile << _T("        Pixel Clock:            ") << (temp / 100.0) << _T("Mhz") << endl;
						int Horizontala= edid.Descriptor[i].DetailDesc.Horizental_active_pixels + (edid.Descriptor[i].DetailDesc.Horizental_pixels_upperbits.bits.Horizental_active_pixels << 8);
						int Horizontalb = edid.Descriptor[i].DetailDesc.Horizontal_blanking_pixels + (edid.Descriptor[i].DetailDesc.Horizental_pixels_upperbits.bits.Horizontal_blanking_pixels << 8);
						outfile << _T("        Horizontal active:          ") << (Horizontala) << _T("pixels") << endl;
						outfile << _T("        Horizontal blanking:        ") << (Horizontalb) << _T("pixels") << endl;
						int Verticala= edid.Descriptor[i].DetailDesc.Vertical_active_pixels + (edid.Descriptor[i].DetailDesc.Vertical_lines_msbits.bits.Vertical_active_pixels << 8);
						int Verticalb = edid.Descriptor[i].DetailDesc.Vertical_blanking_pixels + (edid.Descriptor[i].DetailDesc.Vertical_lines_msbits.bits.Vertical_blanking_pixels << 8);
						outfile << _T("        Vertical  active:           ") << (Verticala) << _T("pixels") << endl;
						outfile << _T("        Vertical  blanking:         ") << (Verticalb) << _T("pixels") << endl;
						int Horizontaloffset = edid.Descriptor[i].DetailDesc.Horizontal_front_porch + (edid.Descriptor[i].DetailDesc.sync_msbits.bits.Horizontal_front_porch << 8);
						int Horizontalwidth = edid.Descriptor[i].DetailDesc.Horizontal_sync_pulse_width + (edid.Descriptor[i].DetailDesc.sync_msbits.bits.Horizontal_sync_pulse_width << 8);
						int Verticaloffset = edid.Descriptor[i].DetailDesc.Vertical_sync_lsbits.bits.Vertical_front_porch + (edid.Descriptor[i].DetailDesc.sync_msbits.bits.Vertical_front_porch << 8);
						int Verticalwidth = edid.Descriptor[i].DetailDesc.Vertical_sync_lsbits.bits.Vertical_sync_pulse_width + (edid.Descriptor[i].DetailDesc.sync_msbits.bits.Vertical_sync_pulse_width << 8);
						outfile << _T("        Horizontal sync. offset:    ") << (Horizontaloffset) << _T("pixels") << endl;
						outfile << _T("        Horizontal sync. width:     ") << (Horizontalwidth) << _T("pixels") << endl;
						outfile << _T("        Vertical sync. offset:      ") << (Verticaloffset) << _T("pixels") << endl;
						outfile << _T("        Vertical sync. width:       ") << (Verticalwidth) << _T("pixels") << endl;
						int Horizonimage = edid.Descriptor[i].DetailDesc.Horizontal_image_size + (edid.Descriptor[i].DetailDesc.Image_Size_msbits.bits.Horizontal_image_size << 8);
						int verticalimage = edid.Descriptor[i].DetailDesc.Vertical_image_size + (edid.Descriptor[i].DetailDesc.Image_Size_msbits.bits.Vertical_image_size << 8);
						outfile << _T("        Horizontal Image Size:      ") << (Horizonimage) << _T(" mm") << endl;
						outfile << _T("        Vertical Image Size:        ") << (verticalimage) << _T("mm") << endl;
						outfile << _T("        Horizontal Border:          ") << (edid.Descriptor[i].DetailDesc.Horizontal_border_pixels) << _T("pixels") << endl;
						outfile << _T("        Vertical Border:            ") << (edid.Descriptor[i].DetailDesc.Vertical_border_lines) << _T("lines") << endl;
					}
				}
			

				outfile.close();
			}
		}

		const DWORD GetEDIDNum()
		{
			return CEDID::EDIDData.ReturnNum();
		}

		void UpdateData()
		{
			CEDID::EDIDData.UpdateData();
			Exec();
		}


	}
}
