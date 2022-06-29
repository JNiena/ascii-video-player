
#include "asciiconverter.hpp"
#include "asciiplayer.hpp"
#include <chrono>
#include <fmt/core.h>
#include <string>
#include <thread>
#include <vector>

int main(int argc, char **argv) {
  if (argc != 4) {
    fmt::print(
      "Invalid arguments. Please provide:\n"
      "1) The path to the video.\n"
      "2) The width of the terminal.\n"
      "3) The height of the terminal.\n"
      "4) How many frames per second the video should run at.\n");
    return -1;
  }

  std::string path = argv[1];
  unsigned int width = std::stoi(argv[2]);
  unsigned int height = std::stoi(argv[3]);
  unsigned int frames_per_second = std::stoi(argv[4]);

  std::vector<std::string> densities {"@", "#", "$", "%", "?", "*", "+", ";", ":", ",", "."};
  AsciiConverter converter(densities);
  AsciiPlayer player;

  std::vector<std::string> frames = converter.from_video(path, width, height);
  player.play_video(frames, frames_per_second);
}