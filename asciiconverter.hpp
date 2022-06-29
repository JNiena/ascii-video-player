#include <functional>
#include <opencv2/core/mat.hpp>
#include <string>
#include <vector>

class AsciiConverter {
  public:
    AsciiConverter(std::vector<std::string> &densities);
    std::string from_image(std::string &path, unsigned int width, unsigned int height);
    std::vector<std::string> from_video(std::string &path, unsigned int width, unsigned int height);

  private:
    std::vector<std::string> &densities;
    std::string frame_to_ascii(cv::Mat &frame);
    void for_each_pixel(cv::Mat &frame, std::function<void(int value, bool newRow)> process);
};