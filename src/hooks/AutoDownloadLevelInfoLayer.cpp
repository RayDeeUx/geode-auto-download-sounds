#include "../constants/SettingsAliases.hpp"
#include "AutoDownloadLevelInfoLayer.hpp"
#include "AutoDownloadCustomSongWidget.hpp"

void AutoDownloadLevelInfoLayer::showDownloadingPopup() {
    if (m_fields->downloadingPopup != nullptr) closeDownloadingPopup();

    auto popup = geode::createQuickPopup(
        "DOWNLOADING AUDIO",
        "Please wait for audio assets to download...",
        "Back", "Skip",
        [this](auto, bool shouldSkip) {
            if (shouldSkip) {
                bool disableSongAlertState = GameManager::get()->getGameVariable(SETTING_DISABLE_SONG_ALERTS);
                if (!disableSongAlertState) {
                    GameManager::get()->setGameVariable(SETTING_DISABLE_SONG_ALERTS, true);
                }
                onPlay(nullptr);
                GameManager::get()->setGameVariable(SETTING_DISABLE_SONG_ALERTS, disableSongAlertState);
            }
        },
        true, true
    );
    m_fields->downloadingPopup = popup;
}

void AutoDownloadLevelInfoLayer::closeDownloadingPopup() {
    log::info("closing");
    auto downloadingPopup = m_fields->downloadingPopup;
    if (downloadingPopup != nullptr) {

        downloadingPopup->removeFromParentAndCleanup(true);
        m_fields->downloadingPopup = nullptr;
    }
}

void AutoDownloadLevelInfoLayer::onPlay(cocos2d::CCObject* sender) {
    LevelInfoLayer::onPlay(sender);

    auto scene = cocos2d::CCDirector::sharedDirector()->getRunningScene();
    if (!scene) return;

    // Close the song alert popup //
    if (!GameManager::get()->getGameVariable(SETTING_DISABLE_SONG_ALERTS)) {
        for (auto child : scene->getChildrenExt()) {
            if (auto alert = typeinfo_cast<FLAlertLayer*>(child)) {
                alert->removeFromParentAndCleanup(true);
            }
        }
    }

    // Trigger download on level play //
    auto* songWidget = this->m_songWidget;
    if (songWidget) {
        auto* autoWidget = static_cast<AutoDownloadCustomSongWidget*>(songWidget);
        if (autoWidget) {
            autoWidget->downloadSongsOnLevelPlay();
        }
    }
}