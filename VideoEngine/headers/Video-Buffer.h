#ifndef VIDEOBUFFER_H
#define VIDEOBUFFER_H

#include "Device-Buffers-Control.h"
#include "V4l2Buffer.h"
#include "Video-Device-Handler.h"

#include <linux/videodev2.h> /* struct v4l2_buffer */

namespace video_streamer {

class VideoBuffer
{
public:
    VideoBuffer(DeviceBuffersControl *device_buffer_control, V4L2Buffer *v4l2buffer, VideoDeviceHandler *video_device_handler)
        : device_buffer_control_ { device_buffer_control },
          v4l2buffer_ {v4l2buffer},
          video_device_handler_ { video_device_handler } {}

    bool MapBufferToMemory();
    bool SetUpBuffer();
    bool UnmapBuffer();

private:
    struct v4l2_buffer video_buffer_;
    DeviceBuffersControl *device_buffer_control_;
    V4L2Buffer *v4l2buffer_;
    VideoDeviceHandler *video_device_handler_;
};

} /* namespace video_streamer */

#endif // VIDEOBUFFER_H
