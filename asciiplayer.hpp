#include <chrono>
#include <string>
#include <vector>

class AsciiPlayer {
  public:
    void play_video(std::vector<std::string> frames, unsigned int frames_per_second);
};