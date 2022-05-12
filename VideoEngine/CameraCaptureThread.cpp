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
	: m(new Private)
{

}

CameraCaptureThread::~CameraCaptureThread()
{
	delete m;
}

void CameraCaptureThread::startCapture()
{
	m->fd = open("/dev/video0", O_RDWR);

	v4l2_format tmpfmt;
	memset(&tmpfmt, 0, sizeof(tmpfmt));
	tmpfmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	ioctl(m->fd, VIDIOC_G_FMT, &tmpfmt);
	m->dstfmt = m->srcfmt = tmpfmt;
	m->dstfmt.fmt.pix.pixelformat = V4L2_PIX_FMT_RGB24;

	m->convert_data = v4lconvert_create(m->fd);
	v4lconvert_try_format(m->convert_data, &m->dstfmt, &m->srcfmt);
	m->srcfmt = tmpfmt;

	struct v4l2_requestbuffers req;
	memset(&req, 0, sizeof(req));
	req.count  = v4l2BufferNum;
	req.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	req.memory = V4L2_MEMORY_MMAP;
	ioctl(m->fd, VIDIOC_REQBUFS, &req);

	struct v4l2_buffer buf;
	for (uint32_t i = 0; i < v4l2BufferNum; i++) {

		memset(&buf, 0, sizeof(buf));
		buf.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = V4L2_MEMORY_MMAP;
		buf.index  = i;
		ioctl(m->fd, VIDIOC_QUERYBUF, &buf);

		m->v4l2Buffer[i] = mmap(NULL, buf.length, PROT_READ, MAP_SHARED, m->fd, buf.m.offset);
		m->v4l2BufferSize[i] = buf.length;
	}

	for (uint32_t i = 0; i < v4l2BufferNum; i++) {
		memset(&buf, 0, sizeof(buf));
		buf.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = V4L2_MEMORY_MMAP;
		buf.index  = i;
		ioctl(m->fd, VIDIOC_QBUF, &buf);
	}

	enum v4l2_buf_type type;
	type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	ioctl(m->fd, VIDIOC_STREAMON, &type);

}

void CameraCaptureThread::copyBuffer()
{
	fd_set fds;
	FD_ZERO(&fds);
	FD_SET(m->fd, &fds);

	while (select(m->fd + 1, &fds, NULL, NULL, NULL) < 0) {
		if (isInterruptionRequested()) return;
	}

	if (FD_ISSET(m->fd, &fds)) {

		struct v4l2_buffer buf;
		memset(&buf, 0, sizeof(buf));
		buf.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = V4L2_MEMORY_MMAP;
		ioctl(m->fd, VIDIOC_DQBUF, &buf);

		{
			QMutexLocker lock(&m->mutex);
			m->image = QImage(m->dstfmt.fmt.pix.width, m->dstfmt.fmt.pix.height, QImage::Format_RGB888);
            v4lconvert_convert(m->convert_data, &m->srcfmt, &m->dstfmt, (unsigned char *)m->v4l2Buffer[buf.index], buf.bytesused, m->image.bits(), m->dstfmt.fmt.pix.sizeimage);
		}

		ioctl(m->fd, VIDIOC_QBUF, &buf);
	}
}

void CameraCaptureThread::stopCapture()
{
	enum v4l2_buf_type type;
	type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	ioctl(m->fd, VIDIOC_STREAMOFF, &type);

	for (uint32_t i = 0; i < v4l2BufferNum; i++) munmap(m->v4l2Buffer[i], m->v4l2BufferSize[i]);

	v4lconvert_destroy(m->convert_data);
	close(m->fd);
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
