#include <Geode/binding/LevelInfoLayer.hpp>
#include <Geode/modify/FLAlertLayer.hpp>
#include <Geode/binding/FLAlertLayer.hpp>

using namespace geode::prelude;

class $modify(NoSongAlertOverride, FLAlertLayer) {
    static LevelInfoLayer* findLevelInfoLayerShallow(CCNode* root) {
        if (!root) return nullptr;
        for (auto n : CCArrayExt<CCNode*>(root->getChildren()))
            if (auto layer = typeinfo_cast<LevelInfoLayer*>(n)) return layer;

        return nullptr;
    }

    static FLAlertLayer* create(
        FLAlertLayerProtocol* delegate,
        char const* title,
        gd::string desc,
        char const* btn1,
        char const* btn2,
        float width,
        bool scroll,
        float height,
        float textScale
    ) {
        auto isAudioWarning = title && std::string_view(title) == "No Song";
        if (isAudioWarning) {
            auto scene = CCDirector::sharedDirector()->getRunningScene();
            if (scene) {
                if (auto* levelInfo = findLevelInfoLayerShallow(scene)) {
                    auto songWidgetErrorLabel = levelInfo->m_songWidget->m_errorLabel;
                    if (songWidgetErrorLabel->isVisible()) {
                        log::info("error with song shown for level info layer, level id {}", levelInfo->m_level->m_levelID);
                        return FLAlertLayer::create(delegate, title, desc, btn1, btn2, width, scroll, height, textScale);
                    }
                }
            }

            title = "Downloading Audio";
            desc = "Please wait for audio assets to download...\n";
            btn1 = "Cancel";
            btn2 = nullptr;
        }

        return FLAlertLayer::create(delegate, title, desc, btn1, btn2, width, scroll, height, textScale);
    }

    void closeWarning() {
        keyBackClicked();
    }
};
