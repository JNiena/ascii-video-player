# Ascii Video Player

### Usage

`ascii-video-player [--help] [--version] --path VAR [--width VAR] [--height VAR] [--framerate VAR] [--blocky] [--audio] [--fill]`

### Args

`--path`: The path to the video. *

`--width`: The display width.

`--height`: The display height.

`--framerate`: The video framerate.

`--audio`: Play audio.

`--fill`: Fill display.

`--blocky`: Use block characters.

### Example

`./ascii-video-player --path video.mp4 --audio --fill`

[Click to download a short example video.](https://github.com/JNiena/ascii-video-player/blob/main/example.mp4)

### Building

```
git clone https://github.com/JNiena/ascii-video-player.git && cd ascii-video-player
mkdir build && cd build
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 ..
cmake --build . --target ascii_video_player
```
