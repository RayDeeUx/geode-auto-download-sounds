#include <Geode/binding/LevelInfoLayer.hpp>
#include <Geode/modify/FLAlertLayer.hpp>
#include <Geode/binding/FLAlertLayer.hpp>
#include "CustomSongWidget.hpp"

using namespace geode::prelude;

class $modify(NoSongAlertOverride, FLAlertLayer) {
    static LevelInfoLayer* findLevelInfoLayerShallow(CCNode* root) {
        if (!root) return nullptr;
        for (auto n : CCArrayExt<CCNode*>(root->getChildren()))
            if (auto layer = typeinfo_cast<LevelInfoLayer*>(n)) return layer;

        return nullptr;
    }

    static LevelInfoLayer* findLevelInfoLayer() {
        auto scene = CCDirector::sharedDirector()->getRunningScene();
        if (scene) {
            return findLevelInfoLayerShallow(scene);
        }
        return nullptr;
    }

    static void trySetLoadingPopupShown(bool shown) {
        auto* levelInfo = findLevelInfoLayer();
        if (!levelInfo) return;

        if (auto* customSongWidget = static_cast<AutoDownloadCustomSongWidget*>(levelInfo->m_songWidget)) {
            customSongWidget->m_fields->m_loadingPopupShown = shown;
        }
    }

    void keyBackClicked() {
        FLAlertLayer::keyBackClicked();
        trySetLoadingPopupShown(false);
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
        if (title && std::string_view(title) == "No Song") {
            auto scene = CCDirector::sharedDirector()->getRunningScene();
            if (scene) {
                if (auto* levelInfo = findLevelInfoLayerShallow(scene)) {
                    auto songWidgetErrorLabel = levelInfo->m_songWidget->m_errorLabel;
                    if (songWidgetErrorLabel->isVisible()) {
                        log::info("error with song shown for level info layer, level id {}", levelInfo->m_level->m_levelID);
                        return FLAlertLayer::create(delegate, title, desc, btn1, btn2, width, scroll, height, textScale);
                    }

                    title = "Downloading Audio";
                    desc = "Please wait for audio assets to download...\n";
                    btn1 = "Cancel";
                    btn2 = "Skip";
                    trySetLoadingPopupShown(true);
                }
            }
        }

        return FLAlertLayer::create(delegate, title, desc, btn1, btn2, width, scroll, height, textScale);
    }
};
