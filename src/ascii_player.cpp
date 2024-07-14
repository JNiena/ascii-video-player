#include "ascii_player.hpp"
#include <format>
#include <iostream>
#include <opencv4/opencv2/imgproc.hpp>
#include <opencv4/opencv2/videoio.hpp>
#include <sys/ioctl.h>
#include <thread>
#include <vector>

namespace ascii_player {
  const std::vector<std::string> densities {"@@", "##", "SS", "%%", "??", "**", "++", ";;", "::", ",,", "  "};
  const std::vector<std::string> densities_blocky {"██", "▓▓", "▒▒", "░░", "  "};

  void play(const std::string& path, int width, int height, bool blocky, bool audio, int framerate = 0) {
    framerate = framerate == 0 ? find_framerate(path) : framerate;

    std::vector<std::string> frames = process(path, width, height, blocky);

    if (audio) {
      play_audio(path);
    }

    std::system("clear");

    for (std::string& frame : frames) {
      std::this_thread::sleep_for(std::chrono::milliseconds(1000 / framerate));
      std::cout << frame;
    }

    std::system("clear");

    std::cout << "Finished playing video.\n";
  }

  std::vector<std::string> process(const std::string& path, int width, int height, bool blocky) {
    cv::VideoCapture capture(path);

    const int resized_width = width / 2;
    const int total_frames = capture.get(cv::CAP_PROP_FRAME_COUNT);
    const std::vector<std::string>& densities = blocky ? ascii_player::densities_blocky : ascii_player::densities;

    std::vector<cv::Mat> frames;

    for (int frame_count = 0; frame_count < total_frames; frame_count++) {
      cv::Mat frame;
      capture >> frame;

      if (frame.empty()) {
        break;
      }

      cv::resize(frame, frame, cv::Size(resized_width, height));
      cv::cvtColor(frame, frame, cv::COLOR_BGR2GRAY);

      frames.push_back(frame);
    }

    std::vector<std::string> processed_frames;

    for (cv::Mat& frame : frames) {
      std::string ascii = "\033[0;0H";

      for (std::size_t row = 0; row < frame.rows; row++) {
        for (std::size_t column = 0; column < frame.cols; column++) {
          const unsigned char pixel_value = frame.at<unsigned char>(row, column);

          ascii += calculate_pixel_density(densities, pixel_value);
        }

        ascii += "\r\n";
      }

      processed_frames.push_back(ascii);
    }

    return processed_frames;
  }

  void play(const std::string& path, bool blocky, bool audio, int framerate = 0) {
    const std::pair<int, int> dimensions = find_dimensions();
    play(path, dimensions.first, dimensions.second - 1, blocky, audio, framerate);
  }

  void play_audio(const std::string& path) {
    std::system(std::format("mpv --no-video \"{}\" > /dev/null 2>&1 &", path).c_str());
  }

  int find_framerate(const std::string& path) {
    return cv::VideoCapture(path).get(cv::CAP_PROP_FPS);
  }

  std::string calculate_pixel_density(const std::vector<std::string>& densities, unsigned char pixel_value) {
    const int density_index = (pixel_value * (densities.size() - 1)) / 255.0;
    return densities[density_index];
  }

  std::pair<int, int> find_dimensions() {
    struct winsize w;
    ioctl(0, TIOCGWINSZ, &w);

    return std::pair<int, int>(w.ws_col, w.ws_row);
  }
}