#ifndef ASCIIPLAYER_H
#define ASCIIPLAYER_H

#include <functional>
#include <string>
#include <vector>

namespace cv {
  class Mat;
}

class AsciiPlayer {
  public:
    const void stream(const std::function<void(const std::string& frame)> callback, const std::string& path, const unsigned int width, const unsigned int height) const;

    const void stream_color(const std::function<void(const std::string& frame)> callback, const std::string& path, const unsigned int width, const unsigned int height) const;
  private:
    const std::string frame_to_ascii(const cv::Mat& frame) const;

    const std::string frame_to_ascii_color(const cv::Mat& frame) const;

    const void for_each_frame(const std::string& path, const std::function<void(cv::Mat& frame)> callback) const;

    template<typename T>
    const void for_each_pixel(const cv::Mat& frame, const std::function<void(const T& value, const unsigned int column, const unsigned int row)> callback) const;
    
    const std::string calculate_pixel_density(const std::vector<std::string>& densities, const unsigned int value) const;
};

#endif
