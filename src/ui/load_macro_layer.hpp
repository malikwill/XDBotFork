#pragma once

#include "../includes.hpp"
#include "../tags.hpp"
#include "record_layer.hpp"
#include <locale>
#include <string>
#include <ctime>

class MacroCell : public CCNode {
	std::string name;
	std::filesystem::path path;
	std::time_t date;
	bool autosave = false;

	geode::Popup* menuLayer = nullptr;
	geode::Popup* mergeLayer = nullptr;
	CCLayer* loadLayer = nullptr;

	bool isMerge = false;

public:

	MacroMetadata metadata;
	std::vector<std::string> tags;

	CCMenu* menu = nullptr;
	CCMenuItemToggler* toggler = nullptr;
	CCLabelBMFont* subLabel = nullptr;

	static MacroCell* create(std::filesystem::path path, std::string name, std::time_t date, MacroMetadata metadata, geode::Popup* menuLayer, geode::Popup* mergeLayer, CCLayer* loadLayer);

	bool init(std::filesystem::path path, std::string name, std::time_t date, MacroMetadata metadata, geode::Popup* menuLayer, geode::Popup* mergeLayer, CCLayer* loadLayer);

	void onLoad(CCObject*);

	void handleLoad();

	void onDelete(CCObject*);

	void deleteMacro(bool reload);

	void onSelect(CCObject*);

	void selectMacro(bool single);

	void onEditTags(CCObject*);

	// Reads this macro's metadata (if not already known) and refreshes the
	// displayed level name in place. Safe to call repeatedly/incrementally -
	// used to spread the cost of reading many macros' metadata across
	// several frames instead of doing it all at once.
	void refreshMetadata();
};

enum class MacroSortMode { Name, Date, Duration };

class LoadMacroLayer : public geode::Popup, public TextInputDelegate {
public:

	geode::Popup* menuLayer = nullptr;
	geode::Popup* mergeLayer = nullptr;
	CCMenu* menu = nullptr;

	CCMenuItemToggler* selectAllToggle = nullptr;
	CCMenuItemToggler* sortToggle = nullptr;
	CCMenuItemSpriteExtra* sortModeBtn = nullptr;
	CCLabelBMFont* sortModeLbl = nullptr;

	CCMenuItemToggler* p1Toggle = nullptr;
	CCMenuItemToggler* p2Toggle = nullptr;
	CCMenuItemToggler* owToggle = nullptr;

	CCMenuItemSpriteExtra* searchOff = nullptr;
	TextInput* searchInput = nullptr;

	CCLabelBMFont* macroCountLbl = nullptr;

	std::vector<MacroCell*> selectedMacros;
	std::vector<MacroCell*> allMacros;
	std::string search = "";

	bool isAutosaves = false;
	bool isMerge = false;
	bool invertSort = false;
	MacroSortMode sortMode = MacroSortMode::Date;

	// Progressive metadata loading - avoids reading every macro file's
	// metadata synchronously on open (which is slow with many macros).
	// Instead the list opens instantly with basic info, and a few files'
	// metadata get read per frame until all cells are enriched.
	size_t metadataProgress = 0;

	static LoadMacroLayer* create(geode::Popup* layer, geode::Popup* layer2, bool autosaves);

	bool setup();

	static void open(geode::Popup* layer, geode::Popup* layer2, bool autosaves = false);

	void openFolder(CCObject*) {
		file::openFolder(Mod::get()->getSettingValue<std::filesystem::path>(isAutosaves ? "autosaves_folder" : "macros_folder"));
	}

	void textChanged(CCTextInputNode* p) override;

	void clearSearch(CCObject*);

	void addList(bool refresh = false, float prevScroll = 0.f);

	void reloadList(int amount = 1);

	void deleteSelected(CCObject*);

	void onSelectAll(CCObject*);

	void onImportMacro(CCObject*);

	void onImportMacroFinished(file::PickResult res);

	void updateSort(CCObject*);

	void cycleSortMode(CCObject*);

	void updateSortModeLabel();

	void processMetadataChunk(float dt);
};