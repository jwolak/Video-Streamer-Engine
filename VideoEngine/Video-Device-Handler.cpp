#include "Video-Device-Handler.h"

#include <sys/fcntl.h> /* O_RDWR */
#include <unistd.h> /* close() */

bool video_streamer::VideoDeviceHandler::OpenDevice(std::string device_path) {

    LOG_DEBUG("%s", "OpenDevice method called");
    LOG_DEBUG("%s%s", "Device path parameter: ", device_path.c_str());

    device_fd_ = open(device_path.c_str(), O_RDWR);

    if (device_fd_ < 0) {

        LOG_ERROR("%s%s%s", "Open the device from path: ", device_path.c_str(), " failed");
        return false;
    }

    LOG_DEBUG("%s%s%s", "Open device form path: ", device_path.c_str(), " successful");
    return true;
}

bool video_streamer::VideoDeviceHandler::CloseDevice() {

    LOG_DEBUG("%s", "CloseDevice method called");

    if (close(device_fd_) < 0) {
        LOG_ERROR("%", "Failed to close device");
        return false;
    }

    LOG_DEBUG("%s", "Device closed successfully");
    return true;
}

int video_streamer::VideoDeviceHandler::GetDeviceFd() {

    LOG_DEBUG("%s", "GetDeviceFd method called");

    return device_fd_;
}
