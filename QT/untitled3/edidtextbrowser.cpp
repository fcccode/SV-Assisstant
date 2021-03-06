#include "edidtextbrowser.h"

edidtextbrowser::edidtextbrowser(QWidget *parent) : QTextBrowser(parent)
{
}

void edidtextbrowser::UpdateData()
{
	this->updateuiedid();
	QTextCursor text_cursor(this->textCursor());
	text_cursor.movePosition(QTextCursor::Start);
	this->setTextCursor(text_cursor);
}

void edidtextbrowser::updateuiedid()
{
    DWORD nums = SV_ASSIST::Display::GetEDIDNum();
    for(DWORD a = 0; a < nums; a++)
    {
        EDID edid = SV_ASSIST::Display::GetEDID(a);
        EDIDManufacturer  buf;
        buf.__Manufacturer = edid.Manufacturer.Manufacturer;
        buf.__Manufacturer = ((buf.__Manufacturer & 0xFF) << 8) | ((buf.__Manufacturer & 0xFF00) >> 8);
        char temp[4];

        temp[0] = buf.bits._First + 'A' - 1;
        temp[1] = buf.bits._Second +'A' - 1;
        temp[2] = buf.bits._Third + 'A' - 1;
        temp[3] = 0;
        this->append(QString(tr("                        EDID ( Extended Display Identification Data) Report #%1").arg(a + 1)));
		emit SendEdidTitle(QString(tr("EDID ( Extended Display Identification Data) Report #%1").arg(a + 1)));
		location.insert(tr("EDID ( Extended Display Identification Data) Report #%1").arg(a + 1), &QTextCursor(this->textCursor()));
		this->append(QString(tr("	Vendor / Product Identification"))); 
		emit SendEdidTitle(QString(tr("Vendor / Product Identification")));
		location.insert(tr("Vendor / Product Identification"), &QTextCursor(this->textCursor()));
        this->append(QString(tr("            Manufacture:                            %1").arg(temp)));
        this->append(QString(tr("            Monitor Serial Number :                 %1").arg(QString::number(edid.SerialNumber, 16)).toUpper()));
        this->append(QString(tr("            Product Code:                           %1").arg(QString::number(edid.Productcode, 16)).toUpper()));
        this->append(QString(tr("            Week/Year:                              %1").arg(edid.Week) +  tr("/") + QString::number(edid.Year + 1990)));
        this->append(QString(tr("            Version:                                %1").arg(edid.EDIDVersion[0]) + tr(".") + QString::number(edid.EDIDVersion[1])));
        this->append(QString(tr("    Video input parameters bitmap")));
		emit SendEdidTitle(QString(tr("Video input parameters bitmap")));
		location.insert(tr("Video input parameters bitmap"), &QTextCursor(this->textCursor()));
        if(edid.VideoInputParameters & 0x80)
        {
        this->append(QString(tr("            Digital input")));
                short bitdepth = (edid.VideoInputParameters & 0x70) >> 4;
                if(bitdepth == 0 || bitdepth == 0x111)
        this->append(QString(tr("            Bit depth:                              Undefined")));
                else
        this->append(QString(tr("            Bit depth:                              %1").arg((bitdepth - 1) * 2 + 6)));
                short videointerface = edid.VideoInputParameters & 0xF;
            switch (videointerface)
            {
            case 1:
        this->append(QString(tr("            Video interface:                        HDMIa")));
                break;
            case 2:
        this->append(QString(tr("            Video interface:                        HDMIb")));
                break;
            case 4:
        this->append(QString(tr("            Video interface:                        MDDI")));
                break;
            case 5:
        this->append(QString(tr("            Video interface:                        DisplayPort")));
                break;
            default:
        this->append(QString(tr("            Video interface:                        Undefined")));
                break;
            }
        }
        else
        {
        this->append(QString(tr("            Analog input")));

        }
        this->append(QString(tr("            Max Horizontal Image Size:              %1 cm").arg(edid.HorizontalScreenSize)));
        this->append(QString(tr("            Max Vertical Image Size:                %1 cm").arg(edid.VerticalScreenSize)));
        this->append(QString(tr("            Gamma:                                  %1") .arg(((edid.DisplayGamma + 100) / 100.0))));
        this->append(QString(tr("    FeatureSupport:")));
        this->append(QString(tr("            Standby:                                ") + (edid.Features & 0x80 ? tr("Yes") : tr("No"))));
        this->append(QString(tr("            Suspend:                                ") + (edid.Features & 0x40 ? tr("Yes") : tr("No"))));
        this->append(QString(tr("            Active off:                             ") + (edid.Features & 0x20 ? tr("Yes") : tr("No"))));
    //    this->append(QString(tr("            standby:                                ") + (edid.Features & 0x80 ? tr("Yes") : tr("No"))));
    //    this->append(QString(tr("            Color space:                            ") + (edid.Features & 0x80 ? tr("Yes") : tr("No"))));
        this->append(QString(tr("            standby:                                ") + (edid.Features & 0x04 ? tr("Yes") : tr("No"))));
        this->append(QString(tr("            Preferred timing mode specified:        ") + (edid.Features & 0x02 ? tr("Yes") : tr("No"))));
        this->append(QString(tr("            Continuous timings with GTF or CVT:     ") + (edid.Features & 0x01 ? tr("Yes") : tr("No"))));
        this->append(QString(tr("    Color Characteristic:")));
		emit SendEdidTitle(QString(tr("Color Characteristic")));
		location.insert(tr("Color Characteristic"), &QTextCursor(this->textCursor()));
        double Red_x = ((edid.RedxValue << 2 )| ((edid.RedAndGreen & 0xC0 ) >> 6 ));
        double Red_y = ((edid.RedyValue<< 2) | ((edid.RedAndGreen & 0x30 ) >> 4));
        double Green_x = ((edid.GreenxyValue[0] << 2 )| ((edid.RedAndGreen & 0x0C ) >> 2));
        double Green_y = ((edid.GreenxyValue[1] << 2)| ((edid.RedAndGreen & 0x03 ) >> 0));
        double Blue_x = ((edid.BluexyValue[0] << 2) | ((edid.BlueAndWhite & 0xC0 ) >> 6));
        double Blue_y = ((edid.BluexyValue[1] << 2 )| ((edid.BlueAndWhite & 0x30 ) >> 4));
        double White_x = ((edid.WhitexyValue[0] << 2 )| ((edid.BlueAndWhite & 0x0C ) >> 2));
        double White_y = ((edid.WhitexyValue[1] << 2 )| ((edid.BlueAndWhite & 0x03 ) >> 0));
       // qDebug() << Red_x << Red_y << Green_x << Green_y << Blue_x << Blue_y << White_x << White_y;
        this->append(QString(tr("            Red:                              X = %1  Y = %2 ").arg(QString::number(Red_x / 1024, 'f', 3)).arg(QString::number(Red_y / 1024, 'f', 3))));
		this->append(QString(tr("            Green:                            X = %1  Y = %2 ").arg(QString::number(Green_x / 1024, 'f', 3)).arg(QString::number(Green_y / 1024, 'f', 3))));
		this->append(QString(tr("            Blue:                             X = %1  Y = %2 ").arg(QString::number(Blue_x / 1024, 'f', 3)).arg(QString::number(Blue_y / 1024, 'f', 3))));
		this->append(QString(tr("            White:                            X = %1  Y = %2 ").arg(QString::number(White_x / 1024, 'f', 3)).arg(QString::number(White_y / 1024, 'f', 3))));
        //...
        this->append(QString(tr("    Established Timings:")));
		emit SendEdidTitle(QString(tr("Established Timings")));
		location.insert(tr("Established Timings"), &QTextCursor(this->textCursor()));
        if(edid.EstablishedTiming[0] & 0x80)
        this->append(QString(tr("            720×400 @ 70 Hz ")));
        if(edid.EstablishedTiming[0] & 0x40)
        this->append(QString(tr("            720×400 @ 88 Hz ")));
        if(edid.EstablishedTiming[0] & 0x20)
        this->append(QString(tr("            640×480 @ 60 Hz ")));
        if(edid.EstablishedTiming[0] & 0x10)
        this->append(QString(tr("            640×480 @ 67 Hz ")));
        if(edid.EstablishedTiming[0] & 0x08)
        this->append(QString(tr("            640×480 @ 72 Hz ")));
        if(edid.EstablishedTiming[0] & 0x04)
        this->append(QString(tr("            640×480 @ 75 Hz ")));
        if(edid.EstablishedTiming[0] & 0x02)
        this->append(QString(tr("            800×600 @ 56 Hz ")));
        if(edid.EstablishedTiming[0] & 0x01)
        this->append(QString(tr("            800×600 @ 60 Hz ")));

        if(edid.EstablishedTiming[1] & 0x80)
        this->append(QString(tr("            800×600 @ 72 Hz ")));
        if(edid.EstablishedTiming[1] & 0x40)
        this->append(QString(tr("            800×600 @ 75 Hz ")));
        if(edid.EstablishedTiming[1] & 0x20)
        this->append(QString(tr("            832×624 @ 75 Hz ")));
        if(edid.EstablishedTiming[1] & 0x10)
        this->append(QString(tr("            1024×768 @ 87 Hz ")));
        if(edid.EstablishedTiming[1] & 0x08)
        this->append(QString(tr("            1024×768 @ 60 Hz ")));
        if(edid.EstablishedTiming[1] & 0x04)
        this->append(QString(tr("            1024×768 @ 72 Hz ")));
        if(edid.EstablishedTiming[1] & 0x02)
        this->append(QString(tr("            1024×768 @ 75 Hz ")));
        if(edid.EstablishedTiming[1] & 0x01)
        this->append(QString(tr("            1280×1024 @ 75 Hz ")));

        if(edid.EstablishedTiming[2] & 0x80)
        this->append(QString(tr("            1152 x 870 @ 75Hz ")));

        for(int i = 0; i < 16; i+=2)
        {
            if(edid.StandardTiming[i] == 0 || (edid.StandardTiming[i] == 1 && 1 == edid.StandardTiming[i + 1]))
        {this->append(QString(tr("    Standard Timing #%1").arg(i / 2)));
        this->append(QString(tr("          Unused")));
        }
            else
               {
        this->append(QString(tr("    Standard Timing #%1").arg(i / 2)));
                int horizontal = (edid.StandardTiming[i] + 31) * 8;
                int RefreshRate  = (edid.StandardTiming[i + 1] & 0x3F) + 60;
                switch(((edid.StandardTiming[i]+1) & 0xC) >> 6)
                {
                case 0:
        this->append(QString(tr("            %1 x %2 @ %3 Hz").arg(horizontal).arg(horizontal / 16 * 10).arg(RefreshRate)));
                    break;
                case 1:
        this->append(QString(tr("            %1 x %1 @ %3 Hz").arg(horizontal).arg(horizontal / 4 * 3).arg(RefreshRate)));
                    break;
                case 2:
        this->append(QString(tr("            %1 x %1 @ %3 Hz").arg(horizontal).arg(horizontal / 5 * 4).arg(RefreshRate)));
                    break;
                case 4:
        this->append(QString(tr("            %1 x %2 @ %3 Hz").arg(horizontal).arg(horizontal / 16 * 9).arg(RefreshRate)));
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
                 QString t = QString::fromUtf8((const char*)edid.Descriptor[i].Display_Serial_Number.Display_Serial_Number, 13);
                switch(OtherDesc.Descriptor_Type)
                {
                case 0xFF:
        this->append(QString(tr("    Display serial number Descriptor:")));
		emit SendEdidTitle(QString(tr("Display serial number Descriptor")));
		location.insert(tr("Display serial number Descriptor"), &QTextCursor(this->textCursor()));
        this->append(QString(tr("        Display serial number:                          %1").arg(t.simplified())));
                    break;
                case 0xFD:
        this->append(QString(tr("    Display Range Limits Descriptor:")));
		emit SendEdidTitle(QString(tr("Display Range Limits Descriptor")));
		location.insert(tr("Display Range Limits Descriptor"), &QTextCursor(this->textCursor()));
        edid.Descriptor[i].Display_Range_Limits_Descriptor.Offsets_for_display_range_limits.Offsets_for_display_range_limits & 0x1 ? minvoffset = 0xFF : minvoffset = 0;
        edid.Descriptor[i].Display_Range_Limits_Descriptor.Offsets_for_display_range_limits.Offsets_for_display_range_limits & 0x2 ? minhoffset = 0xFF : minhoffset = 0;
        edid.Descriptor[i].Display_Range_Limits_Descriptor.Offsets_for_display_range_limits.Offsets_for_display_range_limits & 0x4 ? maxvoffset = 0xFF : maxvoffset = 0;
        edid.Descriptor[i].Display_Range_Limits_Descriptor.Offsets_for_display_range_limits.Offsets_for_display_range_limits & 0x8 ? maxhoffset = 0xFF : maxhoffset =  0;
        this->append(QString(tr("        Min. Horizontal rate:                       %1 khz").arg(edid.Descriptor[i].Display_Range_Limits_Descriptor.Minimum_horizontal_field_rate + minhoffset)));
        this->append(QString(tr("        Max. Horizontal rate:                       %1 khz").arg(edid.Descriptor[i].Display_Range_Limits_Descriptor.Maximum_horizontal_field_rate + maxhoffset)));
        this->append(QString(tr("        Min. Vertical rate:                         %1 khz").arg(edid.Descriptor[i].Display_Range_Limits_Descriptor.Minimum_vertical_field_rate + minvoffset)));
        this->append(QString(tr("        Max. Vertical rate:                         %1 khz").arg(edid.Descriptor[i].Display_Range_Limits_Descriptor.Maximum_vertical_field_rate + maxvoffset)));
        this->append(QString(tr("        Max. Pixel Clock:                           %1 Mhz").arg(edid.Descriptor[i].Display_Range_Limits_Descriptor.Maximum_pixel_clock_rate * 10)));

                    break;
                case 0xFC:
        this->append(QString(tr("    Display name Descriptor:")));
		emit SendEdidTitle(QString(tr("Display name Descriptor")));
		location.insert(tr("Display name Descriptor"), &QTextCursor(this->textCursor()));
        this->append(QString(tr("        Display name number:                          %1").arg(t.simplified())));
        emit SendEdidName(t.simplified());
                    break;
                default:
        this->append(QString(tr("    Descriptors # %1").arg(i)));
        this->append(QString(tr("        To be continued")));
                    break;
                }
            }
            else
            {
        this->append(QString(tr("    Detailed Timing Descriptor:")));
		emit SendEdidTitle(QString(tr("Detailed Timing Descriptor")));
		location.insert(tr("Detailed Timing Descriptor"), &QTextCursor(this->textCursor()));
        int temp = edid.Descriptor[i].DetailDesc.Pixel_Clock;
        //temp = ((temp & 0xFF) << 8) | ((temp & 0xFF00) >> 8);
        this->append(QString(tr("        Pixel Clock:            %1 Mhz").arg(temp/100.0)));
        int Horizontala= edid.Descriptor[i].DetailDesc.Horizental_active_pixels + (edid.Descriptor[i].DetailDesc.Horizental_pixels_upperbits.bits.Horizental_active_pixels << 8);
        int Horizontalb = edid.Descriptor[i].DetailDesc.Horizontal_blanking_pixels + (edid.Descriptor[i].DetailDesc.Horizental_pixels_upperbits.bits.Horizontal_blanking_pixels << 8);
        this->append(QString(tr("        Horizontal active:          %1 Pixels").arg(Horizontala)));
        this->append(QString(tr("        Horizontal blanking:        %1 Pixels").arg(Horizontalb)));
        int Verticala= edid.Descriptor[i].DetailDesc.Vertical_active_pixels + (edid.Descriptor[i].DetailDesc.Vertical_lines_msbits.bits.Vertical_active_pixels << 8);
        int Verticalb = edid.Descriptor[i].DetailDesc.Vertical_blanking_pixels + (edid.Descriptor[i].DetailDesc.Vertical_lines_msbits.bits.Vertical_blanking_pixels << 8);
        this->append(QString(tr("        Vertical  active:           %1 Pixels").arg(Verticala)));
        this->append(QString(tr("        Vertical  blanking:         %1 Pixels").arg(Verticalb)));
        int Horizontaloffset = edid.Descriptor[i].DetailDesc.Horizontal_front_porch + (edid.Descriptor[i].DetailDesc.sync_msbits.bits.Horizontal_front_porch << 8);
        int Horizontalwidth = edid.Descriptor[i].DetailDesc.Horizontal_sync_pulse_width + (edid.Descriptor[i].DetailDesc.sync_msbits.bits.Horizontal_sync_pulse_width << 8);
        int Verticaloffset = edid.Descriptor[i].DetailDesc.Vertical_sync_lsbits.bits.Vertical_front_porch + (edid.Descriptor[i].DetailDesc.sync_msbits.bits.Vertical_front_porch << 8);
        int Verticalwidth = edid.Descriptor[i].DetailDesc.Vertical_sync_lsbits.bits.Vertical_sync_pulse_width + (edid.Descriptor[i].DetailDesc.sync_msbits.bits.Vertical_sync_pulse_width << 8);
        this->append(QString(tr("        Horizontal sync. offset:    %1 pixels").arg(Horizontaloffset)));
        this->append(QString(tr("        Horizontal sync. width:     %1 pixels").arg(Horizontalwidth)));
        this->append(QString(tr("        Vertical sync. offset:      %1 pixels").arg(Verticaloffset)));
        this->append(QString(tr("        Vertical sync. width:       %1 pixels").arg(Verticalwidth)));
        int Horizonimage = edid.Descriptor[i].DetailDesc.Horizontal_image_size + (edid.Descriptor[i].DetailDesc.Image_Size_msbits.bits.Horizontal_image_size << 8);
        int verticalimage = edid.Descriptor[i].DetailDesc.Vertical_image_size + (edid.Descriptor[i].DetailDesc.Image_Size_msbits.bits.Vertical_image_size << 8);
        this->append(QString(tr("        Horizontal Image Size:      %1 mm").arg(Horizonimage)));
        this->append(QString(tr("        Vertical Image Size:        %1 mm").arg(verticalimage)));

        this->append(QString(tr("        Horizontal Border:          %1 pixels").arg(edid.Descriptor[i].DetailDesc.Horizontal_border_pixels)));
        this->append(QString(tr("        Vertical Border:            %1 lines").arg(edid.Descriptor[i].DetailDesc.Vertical_border_lines)));

            }
        }
    }
}

void edidtextbrowser::ReciveEdidTitle(QString x)
{
	bool found = false;
	//想要查找的信息
	//获取文本文档
	QTextDocument *document = this->document();
	//如果不是第一次查找就取消上一次查找操作，响应开始到结束阶段的操作
	if (x.isEmpty())
	{
		return;
	}
	else
	{
		if (x.contains("EDID"))
		{
			x = tr("EDID ( Extended Display Identification Data)");
		}
		//创建文本文档里面的文本光标
		QTextCursor cursor(document);
		QTextCursor editCursor(document);
		//开始
		editCursor.beginEditBlock();
		while (!cursor.isNull() && !cursor.atEnd())
		{
			//个人理解为cursor表示的就是本文里面要查找的内容
			cursor = document->find(x, cursor);
			if (!cursor.isNull())
			{
				found = true;
				this->setTextCursor(cursor);
			}
			//            cursor.movePosition(QTextCursor::WordRight,QTextCursor::KeepAnchor);
			//设置查找信息的颜色，根据函数里的参数类型创建了上面的QTextCharFormat colorFormat
		}
		//结束
		editCursor.endEditBlock();
	}
}
