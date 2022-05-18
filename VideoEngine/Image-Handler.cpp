#include "Image-Handler.h"

QImage & video_streamer::ImageHandler::GetImage() {

    return q_image_;
}

QMutex & video_streamer::ImageHandler::GetImageMutex() {

    return q_mutex_;
}
