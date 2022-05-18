#ifndef IMAGEHANDLER_H
#define IMAGEHANDLER_H

#include <QImage>
#include <QMutex>

namespace video_streamer {

class ImageHandler
{
public:
    ImageHandler() {}
    QImage &GetImage();
    QMutex &GetImageMutex();

private:
    QMutex q_mutex_;
    QImage q_image_;
};

} /* namespace video_streamer */

#endif // IMAGEHANDLER_H
