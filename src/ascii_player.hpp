#ifndef ASCIIPLAYER_H
#define ASCIIPLAYER_H

#include <string>
#include <vector>

namespace cv {
  class Mat;
}

namespace ascii_player {
  void play(const std::string& path, int width, int height, bool blocky, bool audio, int framerate);

  void play(const std::string& path, bool blocky, bool audio, int framerate);

  std::vector<std::string> process(const std::string& path, int width, int height, bool blocky);

  void play_audio(const std::string& path);

  int find_framerate(const std::string& path);

  std::string calculate_pixel_density(const std::vector<std::string>& densities, unsigned char pixel_value);

  std::pair<int, int> find_dimensions();
}

#endif