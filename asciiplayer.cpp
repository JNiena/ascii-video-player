#include "asciiplayer.hpp"
#include <fmt/core.h>
#include <thread>

void AsciiPlayer::play_video(std::vector<std::string> frames, unsigned int frames_per_second) {
  for (unsigned int i = 0; i < frames.size(); i++) {
    fmt::print(frames[i]);
    std::this_thread::sleep_for(std::chrono::milliseconds(frames_per_second));
  }
}