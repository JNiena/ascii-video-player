#include "ascii_player.hpp"
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <fmt/color.h>
#include <sys/ioctl.h>
#include <thread>

const std::vector<std::string> densities_colorless {"@", "#", "S", "%", "?", "*", "+", ";", ":", ",", " "};
const std::vector<std::string> densities_color {"@", "#"};

const void AsciiPlayer::play(const std::string& path, const unsigned int width, const unsigned int height, const bool color, const bool audio, unsigned int framerate = 0) const {
  if (audio) play_audio(path);
  framerate = framerate == 0 ? find_framerate(path) : framerate;
  stream([framerate] (const std::string& frame) {
    auto start = std::chrono::high_resolution_clock::now();
    fmt::print("\033[{};{}H", 0, 0);
    fmt::print(frame);
    auto end = std::chrono::high_resolution_clock::now();
    while (std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() < (1.f / framerate) * 1000) {
      end = std::chrono::high_resolution_clock::now();
    }
  }, path, width, height, color);
}

const void AsciiPlayer::play(const std::string& path, const bool color, const bool audio, unsigned int framerate = 0) const {
  const std::pair<const unsigned int, const unsigned int> dimensions = find_dimensions();
  play(path, dimensions.first, dimensions.second - 1, color, audio, framerate);
}

const void AsciiPlayer::stream_colorless(std::function<void(const std::string& frame)> callback, const std::string& path, const unsigned int width, const unsigned int height) const {
  for_each_frame(path, [&] (cv::Mat& frame) {
    cv::resize(frame, frame, cv::Size(width / 2, height));
    cv::cvtColor(frame, frame, cv::COLOR_BGR2GRAY);
    callback(frame_to_ascii_colorless(frame));
  });
}

const void AsciiPlayer::stream_color(std::function<void(const std::string& frame)> callback, const std::string& path, const unsigned int width, const unsigned int height) const {
  for_each_frame(path, [&] (cv::Mat& frame) {
    cv::resize(frame, frame, cv::Size(width / 2, height));
    cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);
    callback(frame_to_ascii_color(frame));
  });
}

const void AsciiPlayer::stream(const std::function<void(const std::string& frame)> callback, const std::string& path, const unsigned int width, const unsigned int height, const bool color) const {
  if (color) stream_color(callback, path, width, height);
  else stream_colorless(callback, path, width, height);
}

const std::string AsciiPlayer::frame_to_ascii_colorless(const cv::Mat& frame) const {
  std::string ascii;
  for_each_pixel<const unsigned char>(frame, [&] (const unsigned char& value, const unsigned int column, const unsigned int row) {
    const std::string density = calculate_pixel_density(densities_colorless, value);
    ascii += density + density;
    if (column == frame.cols - 1) ascii += "\r\n";
  });
  return ascii;
}

const std::string AsciiPlayer::frame_to_ascii_color(const cv::Mat& frame) const {
  std::string ascii;
  for_each_pixel<const cv::Vec3b>(frame, [&] (const cv::Vec3b& value, const unsigned int column, const unsigned int row) {
    const float brightness = (value[0] * 0.3 + value[1] * 0.6 + value[2] * 0.1);
    const std::string density = calculate_pixel_density(densities_color, brightness);
    ascii += fmt::format(fmt::fg(fmt::rgb(value[0], value[1], value[2])), "{}", density + density);
    if (column == frame.cols - 1) ascii += "\r\n";
  });
  return ascii;
}

const void AsciiPlayer::for_each_frame(const std::string& path, const std::function<void(cv::Mat& frame)> callback) const {
  cv::VideoCapture capture(path);
  for (unsigned int frameCount = 0; frameCount < capture.get(cv::CAP_PROP_FRAME_COUNT); frameCount++) {
    cv::Mat frame;
    capture >> frame;
    callback(frame);
  }
}

template<typename T>
const void AsciiPlayer::for_each_pixel(const cv::Mat& frame, const std::function<void(const T& value, const unsigned int column, const unsigned int row)> callback) const {
  for (unsigned int row = 0; row < frame.rows; row++)  {
    for (unsigned int column = 0; column < frame.cols; column++) {
      callback(frame.at<T>(row, column), column, row);
    }
  }
}

const std::string AsciiPlayer::calculate_pixel_density(const std::vector<std::string>& densities, const unsigned int value) const {
  const float increment = 255.f / densities.size();
  for (float i = 0, j = densities.size() - 1; i <= 255.f; i += increment, j--) {
    if (value >= i && value <= i + increment) {
      return densities[j];
    }
  }
  return densities.back();
}

const void AsciiPlayer::play_audio(const std::string& path) const {
  std::system(fmt::format("ffplay -nodisp {} &", path).c_str());
}

const unsigned int AsciiPlayer::find_framerate(const std::string& path) const {
  return cv::VideoCapture(path).get(cv::CAP_PROP_FPS);
}

const std::pair<const unsigned int, const unsigned int> AsciiPlayer::find_dimensions() const {
  struct winsize w;
  ioctl(0, TIOCGWINSZ, &w);
  return std::pair<const unsigned int, const unsigned int>(w.ws_col, w.ws_row);
}
