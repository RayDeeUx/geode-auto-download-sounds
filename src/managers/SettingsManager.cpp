#include "SettingsManager.hpp"

using namespace geode::prelude;

std::string_view Settings::downloadSoundsWhen() {
    static std::string value = (
        listenForSettingChanges<std::string>("download-sounds-when",[](std::string val) { value = std::move(val); }),
        getMod()->getSettingValue<std::string>("download-sounds-when")
    );
    return value;
}

bool Settings::autoPlayOnDownloadFinish() {
    static bool value = (
        listenForSettingChanges<bool>("play-when-download-finished",[](bool val) { value = std::move(val); }),
        getMod()->getSettingValue<bool>("play-when-download-finished")
    );
    return value;
}

bool Settings::shouldDownloadSoundsOnLevelPlay() {
    return Settings::downloadSoundsWhen() == "Level played";
}

bool Settings::shouldDownloadSoundsOnLevelView() {
    return Settings::downloadSoundsWhen() == "Level viewed";
}

bool Settings::shouldAutoPlayOnDownloadFinish() {
    return Settings::autoPlayOnDownloadFinish();
}