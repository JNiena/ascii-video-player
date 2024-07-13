#ifndef ASCIIPLAYER_H
#define ASCIIPLAYER_H

#include <functional>
#include <string>

namespace cv {
  class Mat;
}

namespace ascii_player {
  void play(const std::string& path, int width, int height, bool audio, int framerate);

  void play(const std::string& path, bool audio, int framerate);

  void stream(const std::function<void(const std::string& frame)> callback, const std::string& path, int width, int height);

  std::string calculate_pixel_density(const std::vector<std::string>& densities, int value);

  void play_audio(const std::string& path);

  int find_framerate(const std::string& path);

  std::pair<int, int> find_dimensions();
}

#endif