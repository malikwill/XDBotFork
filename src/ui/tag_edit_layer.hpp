#pragma once
#include "../includes.hpp"
#include "../tags.hpp"

class TagEditLayer : public geode::Popup {

  TextInput *tagsInput = nullptr;
  std::filesystem::path folder;
  std::string filename;
  CCLayer *loadLayer = nullptr;

  bool setup();

public:
  static TagEditLayer *create(std::filesystem::path const &folder,
                               std::string const &filename,
                               CCLayer *loadLayer);

  void onSave(CCObject *);
};
