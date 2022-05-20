#include "Convert-Data.h"

void video_streamer::ConvertData::DisposeConvertData() {

     v4lconvert_destroy(v4l2_convert_data_);
}

v4lconvert_data *video_streamer::ConvertData::GetConvertData() {

    return v4l2_convert_data_;
}


bool video_streamer::ConvertData::TryFormat(v4l2_format *dst_fmt, v4l2_format *src_fmt) {

    if (v4lconvert_try_format(v4l2_convert_data_, dst_fmt, src_fmt) != 0) {

        LOG_DEBUG("%s", "ConvertData::TryFormat(): v4lconvert_try_format() returned an error");
        LOG_ERROR("%s", "ConvertData::TryFormat() has failed");
        return false;
    }

    return true;
}

bool video_streamer::ConvertData::CreateConvertData() {

    v4l2_convert_data_ = v4lconvert_create(video_device_handler_->GetDeviceFd());

    if (!v4l2_convert_data_) {

        LOG_DEBUG("%s", "ConvertData::CreateConvertData(): v4lconvert_create() returned an error");
        LOG_ERROR("%s", "ConvertData::CreateConvertData(): has failed");
        return false;
    }

    return true;
}
