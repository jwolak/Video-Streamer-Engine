/*
 * Video-Buffer-Tests.cpp
 *
 *  Created on: 2022
 *      Author: Janusz Wolak
 */

/*-
 * BSD 3-Clause License
 *
 * Copyright (c) 2022, Janusz Wolak
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 */

#include "../../../VideoEngine/source/Video-Buffer.cpp"

#include "../Mocks/VideoDeviceHandlerMock.h"
#include "../Mocks/DeviceBuffersControlMock.h"

#include <linux/videodev2.h>

#include <gtest/gtest.h>

#include <memory>
#include <string>
#include <cstring>

using ::testing::Return;
using ::testing::_;

namespace video_buffer_tests {

class VideoBufferTests : public ::testing::Test {
 public:
  VideoBufferTests()
      :
      video_device_handler_mock { new video_engine_tests_mocks::VideoDeviceHandlerMock },
      device_buffers_control_mock { new video_engine_tests_mocks::DeviceBuffersControlMock },
      v42lbuffer { new video_streamer::V4L2Buffer },
      video_buffer { new video_streamer::VideoBuffer { device_buffers_control_mock.get(), v42lbuffer.get(), video_device_handler_mock.get() } } {
  }

  std::shared_ptr<video_engine_tests_mocks::VideoDeviceHandlerMock> video_device_handler_mock;
  std::shared_ptr<video_engine_tests_mocks::DeviceBuffersControlMock> device_buffers_control_mock;
  std::shared_ptr<video_streamer::V4L2Buffer> v42lbuffer;
  std::unique_ptr<video_streamer::VideoBuffer> video_buffer;
};

/* Will not work without interfaces */
TEST_F(VideoBufferTests, DISABLED_Call_MapBufferToMemory) {

  EXPECT_CALL(*device_buffers_control_mock.get(), SetBuffreForDevice(_,_)).WillOnce(Return(false));
  ASSERT_FALSE(video_buffer->MapBufferToMemory());
}

} /*namespace video_buffer_tests*/
