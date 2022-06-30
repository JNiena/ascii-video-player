#ifndef ASCIIPLAYER_H
#define ASCIIPLAYER_H

#include <functional>
#include <string>

namespace cv {
  class Mat;
}

class AsciiPlayer {
  public:
    const void stream(std::function<void(const std::string& frame)> callback, const std::string& path, const unsigned int width, const unsigned int height) const;
  private:
    const std::string frame_to_ascii(const cv::Mat& frame) const;
};

#endif
