#include "CameraCaptureThread.h"

#include <QImage>
#include <QMutex>
#include <unistd.h>
#include <sys/fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <linux/videodev2.h>
#include <QDebug>
#include <libv4lconvert.h>
#include <stdint.h>

static const int v4l2BufferNum = 2;

struct CameraCaptureThread::Private {
	QMutex mutex;
	QImage image;
	int fd;
	void *v4l2Buffer[v4l2BufferNum];
	uint32_t v4l2BufferSize[v4l2BufferNum];
	v4l2_format srcfmt;
	v4l2_format dstfmt;
	v4lconvert_data *convert_data = nullptr;
};

CameraCaptureThread::CameraCaptureThread()
    : m(new Private),
      video_device_handler_ { new video_streamer::VideoDeviceHandler },
      device_buffer_control_ { new video_streamer::DeviceBuffersControl(video_device_handler_)},
      temporary_video_capture_buffer_ {new video_streamer::TemporaryVideoCaptureBuffer },
      request_video_capture_buffer_ { new video_streamer::RequestVideoCaptureBuffer }
{

}

CameraCaptureThread::~CameraCaptureThread()
{
	delete m;
    delete request_video_capture_buffer_;
    delete temporary_video_capture_buffer_;
    delete device_buffer_control_;
    delete video_device_handler_;
}

void CameraCaptureThread::startCapture()
{
    if (!video_device_handler_->OpenDevice("/dev/video0")) {

        LOG_ERROR("%s", "Failed to open the camera device");
        return;
    }

    if (!device_buffer_control_->SetBuffreForDevice(VIDIOC_G_FMT, &temporary_video_capture_buffer_->GetBuffer())) {

        LOG_ERROR("%s", "Failed to set buffer type VIDIOC_G_FMT by SetBuffreForDevice()");
        return;
    }

    m->dstfmt = m->srcfmt = temporary_video_capture_buffer_->GetBuffer();
	m->dstfmt.fmt.pix.pixelformat = V4L2_PIX_FMT_RGB24;

    m->convert_data = v4lconvert_create(video_device_handler_->GetDeviceFd());
	v4lconvert_try_format(m->convert_data, &m->dstfmt, &m->srcfmt);
    m->srcfmt = temporary_video_capture_buffer_->GetBuffer();

    if (!device_buffer_control_->SetBuffreForDevice(VIDIOC_REQBUFS, &request_video_capture_buffer_->GetBuffer())) {

        LOG_ERROR("%s", "Failed to set buffer type VIDIOC_REQBUFS by SetBuffreForDevice");
        return;
    }

	struct v4l2_buffer buf;
	for (uint32_t i = 0; i < v4l2BufferNum; i++) {

		memset(&buf, 0, sizeof(buf));
		buf.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = V4L2_MEMORY_MMAP;
		buf.index  = i;

        if (!device_buffer_control_->SetBuffreForDevice(VIDIOC_QUERYBUF, &buf)) {

            LOG_ERROR("%s", "Failed to set buffer type VIDIOC_QUERYBUF by SetBuffreForDevice");
            return;
        }

        m->v4l2Buffer[i] = mmap(NULL, buf.length, PROT_READ, MAP_SHARED, video_device_handler_->GetDeviceFd(), buf.m.offset);
		m->v4l2BufferSize[i] = buf.length;
	}

	for (uint32_t i = 0; i < v4l2BufferNum; i++) {
		memset(&buf, 0, sizeof(buf));
		buf.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = V4L2_MEMORY_MMAP;
		buf.index  = i;

        if (!device_buffer_control_->SetBuffreForDevice(VIDIOC_QBUF, &buf)) {

            LOG_ERROR("%s", "Failed to set buffer type VIDIOC_QBUF by SetBuffreForDevice");
            return;
        }
	}

	enum v4l2_buf_type type;
	type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    if (!device_buffer_control_->SetBuffreForDevice(VIDIOC_STREAMON, &type)) {

        LOG_ERROR("%s", "Failed to set buffer type VIDIOC_STREAMON by SetBuffreForDevice");
        return;
    }

}

void CameraCaptureThread::copyBuffer()
{
	fd_set fds;
	FD_ZERO(&fds);
    FD_SET(video_device_handler_->GetDeviceFd(), &fds);

    while (select(video_device_handler_->GetDeviceFd() + 1, &fds, NULL, NULL, NULL) < 0) {
		if (isInterruptionRequested()) return;
	}

    if (FD_ISSET(video_device_handler_->GetDeviceFd(), &fds)) {

		struct v4l2_buffer buf;
		memset(&buf, 0, sizeof(buf));
		buf.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = V4L2_MEMORY_MMAP;

        if (!device_buffer_control_->SetBuffreForDevice(VIDIOC_DQBUF, &buf)) {

            LOG_ERROR("%s", "Failed to set buffer type VIDIOC_DQBUF by SetBuffreForDevice");
            return;
        }


		{
			QMutexLocker lock(&m->mutex);
			m->image = QImage(m->dstfmt.fmt.pix.width, m->dstfmt.fmt.pix.height, QImage::Format_RGB888);
            v4lconvert_convert(m->convert_data, &m->srcfmt, &m->dstfmt, (unsigned char *)m->v4l2Buffer[buf.index], buf.bytesused, m->image.bits(), m->dstfmt.fmt.pix.sizeimage);
		}

        if (!device_buffer_control_->SetBuffreForDevice(VIDIOC_QBUF, &buf)) {

            LOG_ERROR("%s", "Failed to set buffer type VIDIOC_QBUF by SetBuffreForDevice");
            return;
        }
	}
}

void CameraCaptureThread::stopCapture()
{
	enum v4l2_buf_type type;
	type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    if (!device_buffer_control_->SetBuffreForDevice(VIDIOC_STREAMOFF, &type)) {

        LOG_ERROR("%s", "Failed to set buffer type VIDIOC_QBUF by SetBuffreForDevice");
        return;
    }

	for (uint32_t i = 0; i < v4l2BufferNum; i++) munmap(m->v4l2Buffer[i], m->v4l2BufferSize[i]);

	v4lconvert_destroy(m->convert_data);

    if (!video_device_handler_->CloseDevice()) {
        LOG_WARNING("%s", "Failed to close the camera device");
    }
}

void CameraCaptureThread::run()
{
	startCapture();
	while (1) {
		if (isInterruptionRequested()) break;
		copyBuffer();
	}
	stopCapture();
}

QImage CameraCaptureThread::image()
{
	QMutexLocker lock(&m->mutex);
	QImage im;
	std::swap(im, m->image);
	return im;
}
