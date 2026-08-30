#include "tag_edit_layer.hpp"
#include "load_macro_layer.hpp"

TagEditLayer *TagEditLayer::create(std::filesystem::path const &folder,
                                    std::string const &filename,
                                    CCLayer *loadLayer) {
  TagEditLayer *ret = new TagEditLayer();
  if (ret->init(240.f, 130.f)) {
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
  menu->setContentSize(m_mainLayer->getContentSize());
  menu->setPosition({0, 0});
  m_mainLayer->addChildAtPosition(menu, Anchor::Center);

  tagsInput = TextInput::create(200, "e.g. speedrun, easy", "chatFont.fnt");
  tagsInput->setString(Tags::join(Tags::get(folder, filename)).c_str());
  menu->addChildAtPosition(tagsInput, Anchor::Center, {0, 20});

  CCLabelBMFont *lbl =
      CCLabelBMFont::create("Comma-separated tags", "chatFont.fnt");
  lbl->setScale(0.5f);
  lbl->setOpacity(120);
  menu->addChildAtPosition(lbl, Anchor::Center, {0, 0});

  ButtonSprite *spr = ButtonSprite::create("Save");
  spr->setScale(0.725f);
  CCMenuItemSpriteExtra *btn = CCMenuItemSpriteExtra::create(
      spr, this, menu_selector(TagEditLayer::onSave));
  menu->addChildAtPosition(btn, Anchor::Center, {0, -30});

  return true;
}

void TagEditLayer::onSave(CCObject *) {
  Tags::set(folder, filename, Tags::parse(tagsInput->getString()));

  this->keyBackClicked();

  if (loadLayer)
    static_cast<LoadMacroLayer *>(loadLayer)->reloadList();

  Notification::create("Tags Saved", NotificationIcon::Success)->show();
}
