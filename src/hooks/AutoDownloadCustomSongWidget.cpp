#include <Geode/Geode.hpp>
#include <Geode/binding/CustomSongDelegate.hpp>
#include <Geode/binding/CustomSongWidget.hpp>
#include <Geode/binding/FLAlertLayer.hpp>
#include <Geode/modify/CustomSongWidget.hpp>
#include <Geode/binding/SongInfoObject.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/binding/MusicDownloadManager.hpp>
#include "../managers/SettingsManager.hpp"
#include "AutoDownloadCustomSongWidget.hpp"
#include "../constants/SettingsAliases.hpp"
#include "AutoDownloadLevelInfoLayer.hpp"

using namespace geode::prelude;

bool AutoDownloadCustomSongWidget::init(SongInfoObject* songInfo, CustomSongDelegate* songDelegate, bool showSongSelect, bool showPlayMusic, bool showDownload, bool isRobtopSong, bool unkBool, bool isMusicLibrary, int unk) {
	if (!CustomSongWidget::init(songInfo, songDelegate, showSongSelect, showPlayMusic, showDownload, isRobtopSong, unkBool, isMusicLibrary, unk)) {
		return false;
	}

	// Download on level view for levels with already fetched song info //
	this->runAction(CCSequence::create(
		CCDelayTime::create(0.f),
		CCCallFunc::create(this, callfunc_selector(AutoDownloadCustomSongWidget::downloadSongsOnLevelView)),
		nullptr
	));

	return true;
}

// Prevents Newgrounds policy crash (@RayDeeUx) //
void AutoDownloadCustomSongWidget::forceAcceptNewgroundsPolicy() {
	if (!GameManager::get()->getGameVariable(SETTING_NEWGROUNDS_POLICY_ACCEPTED)) {
		GameManager::get()->setGameVariable(SETTING_NEWGROUNDS_POLICY_ACCEPTED, true);
	}
}

void AutoDownloadCustomSongWidget::loadSongInfoFinished(SongInfoObject* songInfo) {
	CustomSongWidget::loadSongInfoFinished(songInfo);

	// Download on level view for levels where song info fetching is required //
	downloadSongsOnLevelView();
}

AutoDownloadLevelInfoLayer* AutoDownloadCustomSongWidget::getAutoDownloadLevelInfoLayer() {
	auto scene = cocos2d::CCDirector::sharedDirector()->getRunningScene();
	if (!scene) return nullptr;

	auto* base = scene->getChildByType<LevelInfoLayer>(0);
	if (!base) return nullptr;

	return static_cast<AutoDownloadLevelInfoLayer*>(base);
}

void AutoDownloadCustomSongWidget::downloadSongsOnLevelView() {
	if (!Settings::shouldDownloadSoundsOnLevelView()) return;

	forceAcceptNewgroundsPolicy();

	if (!m_fields->m_startedAutoDownload && m_downloadBtn && m_downloadBtn->isVisible()) {
		m_fields->m_startedAutoDownload = true;
		m_downloadBtn->activate();
	}
}

void AutoDownloadCustomSongWidget::downloadSongsOnLevelPlay() {
	bool downloadButtonVisible = m_downloadBtn && m_downloadBtn->isVisible();
	auto autoDownloadLevelInfoLayer = getAutoDownloadLevelInfoLayer();
	if ((downloadButtonVisible || m_fields->m_startedAutoDownload) && autoDownloadLevelInfoLayer) {
		autoDownloadLevelInfoLayer->showDownloadingPopup();
	}

	if (!Settings::shouldDownloadSoundsOnLevelPlay()) return;

	if (!m_fields->m_startedAutoDownload && downloadButtonVisible) {
		forceAcceptNewgroundsPolicy();
		m_fields->m_startedAutoDownload = true;
		m_downloadBtn->activate();
	}
}

void AutoDownloadCustomSongWidget::tryPlayIfInLevelInfo() {
	auto scene = CCScene::get();
	if (!scene) return;

	auto levelInfo = scene->getChildByType<LevelInfoLayer>(0);
	if (!levelInfo) return;

	levelInfo->onPlay(nullptr);
}

void AutoDownloadCustomSongWidget::allAudiosDownloaded() {
	auto scene = cocos2d::CCDirector::sharedDirector()->getRunningScene();
	if (!scene) return;

	if (auto* base = scene->getChildByType<LevelInfoLayer>(0)) {
		static_cast<AutoDownloadLevelInfoLayer*>(base)->closeDownloadingPopup();
	}

	if (Settings::shouldAutoPlayOnDownloadFinish()) {
		tryPlayIfInLevelInfo();
	}
}

void AutoDownloadCustomSongWidget::showError(bool p0) {
	CustomSongWidget::showError(p0);

	auto levelInfoLayer = CCScene::get()->getChildByType<LevelInfoLayer>(0);
	if (!levelInfoLayer || !levelInfoLayer->m_songWidget || this != levelInfoLayer->m_songWidget) return;

	if (!m_errorLabel->isVisible() || utils::string::toLower(m_errorLabel->getString()) != "download complete.") return;
	if (m_errorLabel->getColor().r != 0 || m_errorLabel->getColor().b != 0 || m_errorLabel->getColor().g != 255) return;

	auto undownloadedAssetCount = m_undownloadedAssets.size();
	if (undownloadedAssetCount == 0 || (undownloadedAssetCount == 1 && m_undownloadedAssets.at(0).m_id == m_songInfoObject->m_songID)) {
		allAudiosDownloaded();
	}
}