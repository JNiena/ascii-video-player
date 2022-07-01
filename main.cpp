#include "ascii_player.hpp"
#include <fmt/core.h>
#include <thread>

int main(int argc, char **argv) {
  if (argc != 5) {
    fmt::print(
      "Invalid arguments. Please provide:\n"
      "1) The path to the video.\n"
      "2) The width of the terminal.\n"
      "3) The height of the terminal.\n"
      "4) How many frames per second the video should run at.\n");
    return -1;
  }
  const std::string path = argv[1];
  const unsigned int width = std::stoi(argv[2]);
  const unsigned int height = std::stoi(argv[3]);
  const unsigned int frames_per_second = std::stoi(argv[4]);
  AsciiPlayer().stream_color([frames_per_second] (const std::string& frame) {
    fmt::print("\033[{};{}H", 0, 0);
    fmt::print(frame);
    std::this_thread::sleep_for(std::chrono::milliseconds(frames_per_second));
  }, path, width, height);
}
