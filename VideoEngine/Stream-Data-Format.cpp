#include "Stream-Data-Format.h"

v4l2_format &video_streamer::StreamDataFormat::GetSrcFormat() {

    return src_frmt_;
}

v4l2_format &video_streamer::StreamDataFormat::GetDstFormat() {

    return dst_frmt_;
}

void video_streamer::StreamDataFormat::SetPixelformat(__u32 pixel_format) {

     dst_frmt_.fmt.pix.pixelformat = pixel_format;
}
