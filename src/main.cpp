#include "ascii_player.hpp"
#include <argparse/argparse.hpp>
#include <filesystem>

int main(int argc, char **argv) {
  argparse::ArgumentParser parser("ascii-video-player");
  parser.add_argument("--path").required().help("The path to the video");
  parser.add_argument("--width").help("The display width.").default_value(0);
  parser.add_argument("--height").help("The display height.").default_value(0);
  parser.add_argument("--framerate").help("The video framerate.").default_value(0);
  parser.add_argument("--audio").default_value(false).implicit_value(true).help("Play audio.");
  parser.add_argument("--fill").default_value(false).implicit_value(true).help("Fill the display.");

  try {
    parser.parse_args(argc, argv);
  }
  catch (const std::exception &err) {
    std::cerr << err.what() << std::endl;
    std::cerr << parser;
    return 1;
  }

  if (!std::filesystem::exists(parser.get<std::string>("path"))) {
    std::cerr << "File not found." << std::endl;
    return 1;
  }

  if (!parser.get<bool>("fill")) {
    if (parser.get<int>("width") < 1) {
      std::cerr << "Width must be greater than zero." << std::endl;
      return 1;
    }

    if (parser.get<int>("height") < 1) {
      std::cerr << "Height must be greater than zero." << std::endl;
      return 1;
    }
  }
  
  if (parser.get<bool>("fill")) {
    ascii_player::play(
      parser.get<std::string>("path"),
      parser.get<bool>("audio"),
      parser.get<int>("framerate")
    );
    return 0;
  }

  ascii_player::play(
    parser.get<std::string>("path"),
    parser.get<int>("width"),
    parser.get<int>("height"),
    parser.get<bool>("audio"),
    parser.get<int>("framerate")
  );

  return 0;
}