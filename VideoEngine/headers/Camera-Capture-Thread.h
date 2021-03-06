#ifndef CAMERACAPTURETHREAD_H
#define CAMERACAPTURETHREAD_H

#include "Video-Device-Handler.h"
#include "Device-Buffers-Control.h"
#include "Temporary-Video-Capture-Buffer.h"
#include "Request-Video-Capture-Buffer.h"
#include "Video-Buffer.h"
#include "V4l2Buffer.h"
#include "Copy-Buffer.h"
#include "Stream-Data-Format.h"
#include "Convert-Data.h"
#include "Image-Handler.h"

#include <QThread>

class CameraCaptureThread : public QThread {
private:
    void StartCapture();
    void CopyBuffer();
    void StopCapture();

protected:
    void run();

public:
	CameraCaptureThread();
	~CameraCaptureThread();
    QImage GetImageFromV4LBuffer();

private:
    video_streamer::VideoDeviceHandler *video_device_handler_;
    video_streamer::DeviceBuffersControl *device_buffer_control_;
    video_streamer::TemporaryVideoCaptureBuffer *temporary_video_capture_buffer_;
    video_streamer::RequestVideoCaptureBuffer *request_video_capture_buffer_;
    video_streamer::V4L2Buffer *v42lbuffer_;
    video_streamer::VideoBuffer *video_buffer_;
    video_streamer::CopyBuffer *copy_buffer_;
    video_streamer::StreamDataFormat *stream_data_format_;
    video_streamer::ConvertData *convert_data_;
    video_streamer::ImageHandler *image_handler_;
};

#endif // CAMERACAPTURETHREAD_H
