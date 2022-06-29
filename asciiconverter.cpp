#include "asciiconverter.hpp"
#include <fmt/core.h>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <string>
#include <vector>

AsciiConverter::AsciiConverter(std::vector<std::string> &densities): densities(densities) {

}

std::string AsciiConverter::from_image(std::string &path, unsigned int width, unsigned int height) {
  cv::Mat frame = cv::imread(path);
  cv::resize(frame, frame, cv::Size(width, height));
  cv::cvtColor(frame, frame, cv::COLOR_BGR2GRAY);
  return frame_to_ascii(frame);
}

std::vector<std::string> AsciiConverter::from_video(std::string &path, unsigned int width, unsigned int height) {
  std::vector<std::string> ascii;
  cv::VideoCapture capture(path);
  unsigned int totalFrames = capture.get(cv::CAP_PROP_FRAME_COUNT);
  for (unsigned int frameCount = 0; frameCount < totalFrames; frameCount++) {
    cv::Mat frame;
    capture >> frame;
    cv::resize(frame, frame, cv::Size(width, height));
    cv::cvtColor(frame, frame, cv::COLOR_BGR2GRAY);
    ascii.push_back(frame_to_ascii(frame));
    fmt::print("Processed frame {} of {}.\n", frameCount, totalFrames);
  }
  return ascii;
}

std::string AsciiConverter::frame_to_ascii(cv::Mat &frame) {
  std::string ascii;
  for_each_pixel(frame, [&](int value, bool newRow) {
    if (newRow) {
      ascii.append("\n");
    }
    float increment = 255.f / this->densities.size();
    for (float i = 0, j = this->densities.size() - 1; i < 255; i += increment, j--) {
      if (value >= i && value <= i + increment) {
        ascii.append(this->densities[j]);
      }
    }
    ascii.append(" ");
  });
  return ascii;
}

void AsciiConverter::for_each_pixel(cv::Mat &frame, std::function<void(int value, bool newRow)> process) {
  for (unsigned int r = 0; r < frame.rows; r++) {
    for (unsigned int c = 0; c < frame.cols; c++) {
      process(frame.at<unsigned char>(r, c), c == frame.cols - 1);
    }
  }
}