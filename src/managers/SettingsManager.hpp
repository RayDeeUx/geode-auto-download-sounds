#pragma once

class Settings {
    private:
        static std::string_view downloadSoundsWhen();
        static bool autoPlayOnDownloadFinish();
        static bool autoAcceptNewgroundsPolicy();
    public:
        static bool shouldDownloadSoundsOnLevelView();
        static bool shouldDownloadSoundsOnLevelPlay();
        static bool shouldDownloadSoundsNever();
        static bool shouldAutoPlayOnDownloadFinish();
        static bool shouldAutoAcceptNewgroundsPolicy();
};