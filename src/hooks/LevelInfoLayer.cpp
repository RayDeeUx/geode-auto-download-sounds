#include <Geode/modify/LevelInfoLayer.hpp>
#include <Geode/binding/LevelInfoLayer.hpp>
#include "../managers/SettingsManager.hpp"
#include "CustomSongWidget.hpp"

using namespace geode::prelude;

class $modify(MyLevelInfoLayer, LevelInfoLayer) {
    void onPlay(cocos2d::CCObject* sender) {
        LevelInfoLayer::onPlay(sender);

        auto scene = cocos2d::CCDirector::sharedDirector()->getRunningScene();
        if (!scene) return;

        // Close the song alert popup //
        
        auto children = scene->getChildren();
        for (int i = 0; i < children->count(); ++i) {
            if (auto alert = dynamic_cast<FLAlertLayer*>(children->objectAtIndex(i))) {
                alert->keyBackClicked();
            }
        }

        if (Settings::shouldDownloadSoundsOnLevelPlay()) {
            auto* songWidget = this->m_songWidget;
            if (songWidget) {
                auto* autoWidget = static_cast<AutoDownloadCustomSongWidget*>(songWidget);
                if (autoWidget) {
                    autoWidget->downloadSongsOnLevelPlay();
                }
            }
        }

    }
};
