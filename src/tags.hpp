#pragma once

#include <filesystem>
#include <string>
#include <vector>

// Local, per-folder tag storage for the macro browser. Tags are arbitrary
// user-defined labels attached to a macro file, stored in a small sidecar
// "xdbot_tags.json" file placed inside the same folder as the macros
// themselves (so macros_folder and autosaves_folder each get their own).
// This is purely local metadata - it never touches the macro file itself.
namespace Tags {

// Returns the tags for a given macro filename (e.g. "run1.gdr2"), or an
// empty vector if it has none / the folder has no tags file yet.
std::vector<std::string> get(std::filesystem::path const &folder,
                              std::string const &filename);

// Overwrites the tag list for a given macro filename and persists it to
// disk immediately. Passing an empty vector removes the entry entirely.
void set(std::filesystem::path const &folder, std::string const &filename,
         std::vector<std::string> const &tags);

// Splits a raw comma-separated string (as typed by the user) into a
// trimmed, lowercased, deduplicated, non-empty tag list.
std::vector<std::string> parse(std::string const &raw);

// Joins a tag list back into a comma-and-space-separated string, suitable
// for showing in an edit field or a compact display line.
std::string join(std::vector<std::string> const &tags);

} // namespace Tags
