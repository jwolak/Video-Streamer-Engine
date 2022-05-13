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
    bool SetBuffreForDevice(unsigned long int buffer_type, void *buffer);

private:
    VideoDeviceHandler* video_device_handler_;
};

} /*namespace video_streamer */

#endif // DEVICEBUFFERSCONTROL_H
