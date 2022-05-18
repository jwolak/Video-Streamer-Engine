#include "Convert-Data.h"

bool video_streamer::ConvertData::DisposeConvertData() {

     v4lconvert_destroy(v4l2_convert_data_);

     return true;
}

v4lconvert_data * video_streamer::ConvertData::GetConvertData() {

    return v4l2_convert_data_;
}


bool video_streamer::ConvertData::TryFormat(v4l2_format *dst_fmt, v4l2_format *src_fmt) {

    v4lconvert_try_format(v4l2_convert_data_, dst_fmt, src_fmt);

    return true;
}

bool video_streamer::ConvertData::CreateConvertData() {

    v4l2_convert_data_ = v4lconvert_create(video_device_handler_->GetDeviceFd());

    return true;
}
