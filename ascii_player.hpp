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
    const void play(const std::string& path, const unsigned int width, const unsigned int height, const bool color, const bool audio, unsigned int framerate) const;

    const void play(const std::string& path, const bool color, const bool audio, unsigned int framerate) const;
  private:
    const void stream_colorless(const std::function<void(const std::string& frame)> callback, const std::string& path, const unsigned int width, const unsigned int height) const;

    const void stream_color(const std::function<void(const std::string& frame)> callback, const std::string& path, const unsigned int width, const unsigned int height) const;

    const void stream(const std::function<void(const std::string& frame)> callback, const std::string& path, const unsigned int width, const unsigned int height, const bool color) const;

    const std::string frame_to_ascii_colorless(const cv::Mat& frame) const;

    const std::string frame_to_ascii_color(const cv::Mat& frame) const;

    const void for_each_frame(const std::string& path, const std::function<void(cv::Mat& frame)> callback) const;

    template<typename T>
    const void for_each_pixel(const cv::Mat& frame, const std::function<void(const T& value, const unsigned int column, const unsigned int row)> callback) const;

    const std::string calculate_pixel_density(const std::vector<std::string>& densities, const unsigned int value) const;

    const void play_audio(const std::string& path) const;

    const unsigned int find_framerate(const std::string& path) const;

    const std::pair<const unsigned int, const unsigned int> find_dimensions() const;
};

#endif
