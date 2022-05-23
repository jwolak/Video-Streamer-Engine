#include "headers/Device-Buffers-Control.h"

#include "Logger.h"

#include <sys/ioctl.h> /*ioctl()*/

bool video_streamer::DeviceBuffersControl::SetBuffreForDevice(unsigned long int buffer_type, void *buffer) {

    if (ioctl(video_device_handler_->GetDeviceFd(), buffer_type, buffer) < 0) {

        LOG_DEBUG("%s", "Function ioctl() error returned");
        LOG_ERROR("%s", "Failed to set buffer type");
        return false;
    }

    return true;
}
