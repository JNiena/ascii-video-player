#include "ascii_player.hpp"
#include <fmt/core.h>
#include <SFML/Audio.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>

const std::vector<std::string> densities {"@", "#", "$", "%", "?", "*", "+", ";", ":", ",", "."};
const float increment = 255.f / densities.size();

const void AsciiPlayer::stream(std::function<void(const std::string& frame)> callback, const std::string& path, const unsigned int width, const unsigned int height) const {
  std::system(fmt::format("ffmpeg -loglevel quiet -i {} -ac 2 -f wav audio.wav -y", path).c_str());
  sf::Music audio;
  audio.openFromFile("audio.wav");
  audio.play();
  cv::VideoCapture capture(path);
  const unsigned int totalFrames = capture.get(cv::CAP_PROP_FRAME_COUNT);
  for (unsigned int frameCount = 0; frameCount < totalFrames; frameCount++) {
    cv::Mat frame;
    capture >> frame;
    cv::resize(frame, frame, cv::Size(width, height));
    cv::cvtColor(frame, frame, cv::COLOR_BGR2GRAY);
    callback(frame_to_ascii(frame));
  }
}

const std::string AsciiPlayer::frame_to_ascii(const cv::Mat& frame) const {
  std::string ascii;
  for (unsigned int row = 0; row < frame.rows; row++)  {
    for (unsigned int column = 0; column < frame.cols; column++) {
      const unsigned char value = frame.at<unsigned char>(row, column);
      if (column == frame.cols - 1) ascii += "\n";
      for (float i = 0, j = densities.size() - 1; i < 255; i += increment, j--) {
        if (value >= i && value <= i + increment) ascii.append(densities[j]);
      }
      ascii.append(" ");
    }
  }
  return ascii;
}
