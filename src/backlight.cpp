#include <libluu/backlight.hpp>
#include <libluu/utility.hpp>
#include <version.hpp>
namespace backlight {
  using namespace utility;

  vector<path> default_paths() {
    vector<path> returner{{"/etc/backlight/config"}};
    returner.push_back(backlight::get_xdg_config_path());
    return returner;
  }
/** takes a sysfs path to a backlight device and returns current brightness in
 * terms of percent of maximum brightness. */
float get_current_brightness_percentage(const path device) {
  int max = get_int_from_file(device / "max_brightness");
  int cur = get_int_from_file(device / "actual_brightness");
  return utility::ez_pct(cur, max);
}

void adjust_brightness_by_increment(const path path, const int percentage) {
  int max = get_int_from_file(path / "max_brightness");
  int prev = get_int_from_file(path / "actual_brightness");
  int min = 0;
  int increment = max * percentage;
  increment = increment / 100;

  /* ensure that 1percent does at least the bare minimum for devices with a low
   * max_brightness*/
  if (percentage > 0) {
    if (increment == 0) {
      increment = 1;
    }
  } else if (percentage < 0) {
    if (increment == 0) {
      increment = -1;
    }
  } else { // this is silly
    std::cerr << "hodor";
  }
  int target_value = prev + increment;

  /* ensure that we keep our crayons within the lines */
  if (target_value < min)
    target_value = min;
  if (target_value > max)
    target_value = max;
  put_int_to_file(target_value, path / "brightness");
}

void adjust_brightness_to_target_percentage(const path path,
                                            const int percentage) {
  if (percentage == 0) {
    put_int_to_file(0, path / "brightness");
  } else {
    int max = get_int_from_file(path / "max_brightness");
    int target_value = max * percentage;
    target_value = target_value / 100;
    if (target_value > max)
      target_value = max;
    put_int_to_file(target_value, path / "brightness");
  }
}

path get_xdg_config_path() {
  path xdg_config_dir{};
  if (const char *env_p = std::getenv("HOME")) {
    xdg_config_dir = path{env_p} / ".config/backlight/config";
  }
  return xdg_config_dir;
}

vector<path> get_backlights_from_config_file(path config) {
  vector<path> returner;
  ifstream infile(config);
  string empty, line = string{};
  while (infile.good()) {
    getline(infile, line);
    if (line != empty) {
      returner.push_back(line);
    }
    line.clear();
  }
  return returner;
}

/*inline vector<string> filename_to_vector_of_strings(path filename) {
  vector<string> returner{};
  // Open the File
  ifstream in(filename);
  string str;
  // Check if object is valid
  if (!in) {
    dbg(true, 0, "Cannot open the File:", filename);
  } else {
    dbg(true, 0, "File opened:", filename);

    // Read the next line from File untill it reaches the end.
    while (getline(in, str)) {
      // Line contains string of length > 0 then save it in vector
      if (str.size() > 0)
        returner.push_back(str);
    }
  }
  // Close The File
  in.close();
  return returner;
}*/

vector<path> scan_for_valid_backlights() {
  vector<path> returner{};
  path sysfs_backlight_root = "/sys/class/backlight";
  if (fs::exists(sysfs_backlight_root)) {
    for (path device_path : fs::directory_iterator(sysfs_backlight_root)) {
      if (fs::exists(device_path / "max_brightness") and
          fs::exists(device_path / "actual_brightness") and
          fs::exists(device_path / "brightness")) 
      {
        bool already_exists = false;
        for (auto it : returner){
          if (device_path == it) {
            already_exists = true;
          }
        }
        if (!already_exists) {
          returner.emplace_back(device_path);
        }
      }
    }
  }
  return returner;
}
vector<path> scan_for_valid_backlights_unique() {
  //TODO, template this with vector version
  vector<path> returner;
  for (auto it : scan_for_valid_backlights()){
    returner.emplace_back(it);
  }
  return returner;
}
}