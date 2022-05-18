#ifndef CONVERTDATA_H
#define CONVERTDATA_H

#include "Video-Device-Handler.h"

#include <libv4lconvert.h>

namespace video_streamer {

class ConvertData
{
public:
    ConvertData(VideoDeviceHandler *video_device_handler) : video_device_handler_ {video_device_handler} {
    }

    bool CreateConvertData();
    bool DisposeConvertData();
    v4lconvert_data *GetConvertData();
    bool TryFormat(v4l2_format *dst_fmt, v4l2_format *src_fmt);

private:
    VideoDeviceHandler *video_device_handler_;
    v4lconvert_data *v4l2_convert_data_;
};

} /* namespace video_streamer */

#endif // CONVERTDATA_H
