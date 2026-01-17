#pragma once

#include <Geode/modify/LevelInfoLayer.hpp>
#include <Geode/binding/LevelInfoLayer.hpp>

using namespace geode::prelude;

class $modify(AutoDownloadLevelInfoLayer, LevelInfoLayer) {
    struct Fields {
        FLAlertLayer* downloadingPopup = nullptr;
    };

    void onPlay(CCObject* sender);
    void showDownloadingPopup();
    void closeDownloadingPopup();
};
