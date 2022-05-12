#ifndef VIDEODEVICEHANDLER_H
#define VIDEODEVICEHANDLER_H

#include "Logger.h"

#include <string>

namespace video_streamer {

class VideoDeviceHandler
{
public:
    VideoDeviceHandler() : device_fd_{ -1 } {}
    bool OpenDevice(std::string device_path);
    bool CloseDevice();
    int GetDeviceFd();

private:
    int device_fd_;
};

} /*namespace video_streamer*/

#endif // VIDEODEVICEHANDLER_H
