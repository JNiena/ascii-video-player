#include "ascii_player.hpp"
#include <format>
#include <iostream>
#include <opencv4/opencv2/imgproc.hpp>
#include <opencv4/opencv2/videoio.hpp>
#include <sys/ioctl.h>

namespace ascii_player {
  const std::vector<std::string> densities {"@@", "##", "SS", "%%", "??", "**", "++", ";;", "::", ",,", "  "};

  void play(const std::string& path, int width, int height, bool audio, int framerate = 0) {
    if (audio) {
      play_audio(path);
    }

    framerate = framerate == 0 ? find_framerate(path) : framerate;

    cv::VideoCapture capture(path);

    for (int frame_count = 0; frame_count < capture.get(cv::CAP_PROP_FRAME_COUNT) - 1; frame_count++) {
      auto start = std::chrono::high_resolution_clock::now();

      cv::Mat frame;
      capture >> frame;

      cv::resize(frame, frame, cv::Size(width / 2, height));
      cv::cvtColor(frame, frame, cv::COLOR_BGR2GRAY);

      std::string ascii;

      for (std::size_t row = 0; row < frame.rows; row++) {
        for (std::size_t column = 0; column < frame.cols; column++) {
          const std::string density = calculate_pixel_density(densities, frame.at<unsigned char>(row, column));
          ascii += density;
          
          if (column == frame.cols - 1) {
            ascii += "\r\n";
          }
        }
      }

      std::cout << "\033[0;0H";
      std::cout << ascii;

      auto end = std::chrono::high_resolution_clock::now();

      while (std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() < (1.0 / framerate) * 1000) {
        end = std::chrono::high_resolution_clock::now();
      }
    }

    std::system("clear");
    std::cout << "Finished playing video.\n";
  }

  void play(const std::string& path, bool audio, int framerate = 0) {
    const std::pair<int, int> dimensions = find_dimensions();
    play(path, dimensions.first, dimensions.second - 1, audio, framerate);
  }

  void play_audio(const std::string& path) {
    std::system(std::format("mpv --no-video \"{}\" > /dev/null 2>&1 &", path).c_str());
  }

  int find_framerate(const std::string& path) {
    return cv::VideoCapture(path).get(cv::CAP_PROP_FPS);
  }

  std::string calculate_pixel_density(const std::vector<std::string>& densities, int value) {
    const double increment = 255.0 / densities.size();

    for (double i = 0, j = densities.size() - 1; i <= 255.f; i += increment, j--) {
      if (value >= i && value <= i + increment) {
        return densities[j];
      }
    }

    return densities.back();
  }

  std::pair<int, int> find_dimensions() {
    struct winsize w;
    ioctl(0, TIOCGWINSZ, &w);

    return std::pair<int, int>(w.ws_col, w.ws_row);
  }
}