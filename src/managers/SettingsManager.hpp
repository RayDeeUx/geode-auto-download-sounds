#pragma once

class Settings {
    private:
        static std::string_view downloadSoundsWhen();
    public:
        static bool shouldDownloadSoundsOnLevelView();
        static bool shouldDownloadSoundsOnLevelPlay();
};