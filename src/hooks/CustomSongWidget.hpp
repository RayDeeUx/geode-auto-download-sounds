#pragma once

#include <Geode/modify/CustomSongWidget.hpp>
#include <Geode/binding/SongInfoObject.hpp>
#include <Geode/binding/CustomSongDelegate.hpp>

using namespace geode::prelude;

class $modify(AutoDownloadCustomSongWidget, CustomSongWidget) {
public:
    struct Fields {
        bool m_startedAutoDownload = false;
        bool m_loadingPopupShown = false;
    };

    bool init(
        SongInfoObject* songInfo,
        CustomSongDelegate* songDelegate,
        bool showSongSelect,
        bool showPlayMusic,
        bool showDownload,
        bool isRobtopSong,
        bool unkBool,
        bool isMusicLibrary,
        int unk
    );
    void loadSongInfoFinished(SongInfoObject* songInfo);
    void downloadSongsOnLevelPlay();
    void downloadSongsOnLevelView();
    void tryPlayIfInLevelInfo();
    void allAudiosDownloaded();
    /*
    void downloadSongFinished(int id);
    void downloadSFXFinished(int id);
    */
    void showError(bool p0);
};
