#include <Geode/modify/LevelInfoLayer.hpp>
#include <Geode/binding/LevelInfoLayer.hpp>
#include "../managers/SettingsManager.hpp"
#include "CustomSongWidget.hpp"

using namespace geode::prelude;

class $modify(MyLevelInfoLayer, LevelInfoLayer) {
    void onPlay(cocos2d::CCObject* sender) {
        if (Settings::shouldDownloadSoundsOnLevelPlay()) {
            auto* songWidget = this->m_songWidget;
            if (songWidget) {
                auto* autoWidget = static_cast<AutoDownloadCustomSongWidget*>(songWidget);
                if (autoWidget) {
                    autoWidget->downloadSongsOnLevelPlay();
                }
            }
        }

        LevelInfoLayer::onPlay(sender);
    }
};
