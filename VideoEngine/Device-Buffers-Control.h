#ifndef DEVICEBUFFERSCONTROL_H
#define DEVICEBUFFERSCONTROL_H

#include "Video-Device-Handler.h"

#include <string>
#include <cstdint>

namespace video_streamer {

class DeviceBuffersControl
{
public:
    DeviceBuffersControl(VideoDeviceHandler* video_device_handler) : video_device_handler_ { video_device_handler } {}

private:
    VideoDeviceHandler* video_device_handler_;
};

} /*namespace video_streamer */

#endif // DEVICEBUFFERSCONTROL_H
