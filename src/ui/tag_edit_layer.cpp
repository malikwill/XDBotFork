#include "tag_edit_layer.hpp"
#include "load_macro_layer.hpp"

TagEditLayer *TagEditLayer::create(std::filesystem::path const &folder,
                                    std::string const &filename,
                                    CCLayer *loadLayer) {
  TagEditLayer *ret = new TagEditLayer();
  if (ret->init(240, 130, Utils::getTexture().c_str())) {
    ret->folder = folder;
    ret->filename = filename;
    ret->loadLayer = loadLayer;
    ret->setup();
    ret->autorelease();
    return ret;
  }

  delete ret;
  return nullptr;
}

bool TagEditLayer::setup() {
  setTitle("Edit Tags");

  CCMenu *menu = CCMenu::create();
  m_mainLayer->addChild(menu);

  tagsInput = TextInput::create(200, "e.g. speedrun, easy", "chatFont.fnt");
  tagsInput->setPosition({120, 78});
  tagsInput->setString(Tags::join(Tags::get(folder, filename)).c_str());
  menu->addChild(tagsInput);

  CCLabelBMFont *lbl =
      CCLabelBMFont::create("Comma-separated tags", "chatFont.fnt");
  lbl->setPosition({120, 58});
  lbl->setScale(0.5f);
  lbl->setOpacity(120);
  menu->addChild(lbl);

  ButtonSprite *spr = ButtonSprite::create("Save");
  spr->setScale(0.725f);
  CCMenuItemSpriteExtra *btn = CCMenuItemSpriteExtra::create(
      spr, this, menu_selector(TagEditLayer::onSave));
  btn->setPosition({120, 28});
  menu->addChild(btn);

  return true;
}

void TagEditLayer::onSave(CCObject *) {
  Tags::set(folder, filename, Tags::parse(tagsInput->getString()));

  this->keyBackClicked();

  if (loadLayer)
    static_cast<LoadMacroLayer *>(loadLayer)->reloadList();

  Notification::create("Tags Saved", NotificationIcon::Success)->show();
}
