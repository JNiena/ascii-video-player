#include "ascii_player.hpp"
#include <lyra/lyra.hpp>
#include <fmt/core.h>
#include <filesystem>

int main(int argc, char **argv) {
  std::string path = "";
  unsigned int width = 0;
  unsigned int height = 0;
  unsigned int framerate = 0;
  bool color = false;
  bool audio = false;
  bool fill = false;
  bool preprocess = false;
  lyra::cli parser = lyra::cli()
    | lyra::opt(path, "path") ["-p"]["--path"]("The path to the video.").required()
    | lyra::opt(width, "width") ["-w"]["--width"]("The display width.").optional()
    | lyra::opt(height, "height") ["-h"]["--height"]("The display height.").optional()
    | lyra::opt(framerate, "framerate") ["-f"]["--framerate"]("The framerate of the video.").optional()
    | lyra::opt(color) ["-c"]["--color"]("Display color.").optional()
    | lyra::opt(audio) ["-a"]["--audio"]("Play audio.").optional()
    | lyra::opt(fill) ["--fill"]("Fills the display.").optional();
  lyra::parse_result result = parser.parse({argc, argv});
  if (result) {
    if (!std::filesystem::exists(path)) {
      fmt::print("The file has not been found.\n");
      return -1;
    }
    if (fill) {
      AsciiPlayer().play(path, color, audio, framerate);
      return 0;
    }
    if (!(width > 0)) {
      fmt::print("The width must be greater than zero.\n");
      return -1;
    }
    if (!(height > 0)) {
      fmt::print("The height must be greater than zero.\n");
      return -1;
    }
    AsciiPlayer().play(path, width, height, color, audio, framerate);
  } else {
    fmt::print("{}\n", result.message());
  }
}
