#include "tags.hpp"

#include "gdr/json.hpp"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <unordered_map>

namespace {

using json = nlohmann::json;

std::filesystem::path tagsFilePath(std::filesystem::path const &folder) {
  return folder / "xdbot_tags.json";
}

// One in-memory cache entry per folder, loaded lazily and kept in sync with
// every write so repeated get() calls (e.g. rebuilding the whole macro list)
// don't re-read the sidecar file from disk each time.
std::unordered_map<std::string, json> folderCache;

json &loadFolder(std::filesystem::path const &folder) {
  std::string key = folder.string();

  auto it = folderCache.find(key);
  if (it != folderCache.end())
    return it->second;

  json data = json::object();

  std::ifstream f(tagsFilePath(folder).string());
  if (f) {
    json parsed = json::parse(f, nullptr, false);
    if (!parsed.is_discarded() && parsed.is_object())
      data = parsed;
  }

  return folderCache[key] = data;
}

void saveFolder(std::filesystem::path const &folder, json const &data) {
  std::ofstream f(tagsFilePath(folder).string());
  if (f)
    f << data.dump(2);
}

} // namespace

namespace Tags {

std::vector<std::string> get(std::filesystem::path const &folder,
                              std::string const &filename) {
  json &data = loadFolder(folder);

  auto it = data.find(filename);
  if (it == data.end() || !it->is_array())
    return {};

  std::vector<std::string> result;
  for (auto const &tag : *it) {
    if (tag.is_string())
      result.push_back(tag.get<std::string>());
  }

  return result;
}

void set(std::filesystem::path const &folder, std::string const &filename,
         std::vector<std::string> const &tags) {
  json &data = loadFolder(folder);

  if (tags.empty())
    data.erase(filename);
  else
    data[filename] = tags;

  saveFolder(folder, data);
}

std::vector<std::string> parse(std::string const &raw) {
  std::vector<std::string> result;
  std::string current;

  auto flush = [&]() {
    // Trim whitespace
    size_t start = current.find_first_not_of(" \t\n\r");
    size_t end = current.find_last_not_of(" \t\n\r");

    if (start == std::string::npos) {
      current.clear();
      return;
    }

    std::string trimmed = current.substr(start, end - start + 1);
    std::transform(trimmed.begin(), trimmed.end(), trimmed.begin(),
                    [](unsigned char c) { return std::tolower(c); });

    if (!trimmed.empty() &&
        std::find(result.begin(), result.end(), trimmed) == result.end())
      result.push_back(trimmed);

    current.clear();
  };

  for (char c : raw) {
    if (c == ',') {
      flush();
    } else {
      current += c;
    }
  }
  flush();

  return result;
}

std::string join(std::vector<std::string> const &tags) {
  std::string result;
  for (size_t i = 0; i < tags.size(); ++i) {
    if (i > 0)
      result += ", ";
    result += tags[i];
  }
  return result;
}

} // namespace Tags
