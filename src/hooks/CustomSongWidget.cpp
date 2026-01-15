#include <Geode/Geode.hpp>
#include <Geode/binding/CustomSongDelegate.hpp>
#include <Geode/binding/CustomSongWidget.hpp>
#include <Geode/binding/FLAlertLayer.hpp>
#include <Geode/modify/CustomSongWidget.hpp>
#include <Geode/binding/SongInfoObject.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/binding/MusicDownloadManager.hpp>
#include "../managers/SettingsManager.hpp"
#include "CustomSongWidget.hpp"

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

void AutoDownloadCustomSongWidget::loadSongInfoFinished(SongInfoObject* songInfo) {
	CustomSongWidget::loadSongInfoFinished(songInfo);

	// Download on level view for levels where song info fetching is required //
	downloadSongsOnLevelView();
}

void AutoDownloadCustomSongWidget::downloadSongsOnLevelView() {
	if (!GameManager::get()->getGameVariable("0016")) GameManager::get()->setGameVariable("0016", true);
	bool orig83 = GameManager::get()->getGameVariable("0083");
	GameManager::get()->setGameVariable("0083", true);
	if (!m_fields->m_startedAutoDownload && Settings::shouldDownloadSoundsOnLevelView() && m_downloadBtn && m_downloadBtn->isVisible()) {
		m_fields->m_startedAutoDownload = true;
		m_downloadBtn->activate();
	}
	GameManager::get()->setGameVariable("0083", orig83);
}

void AutoDownloadCustomSongWidget::downloadSongsOnLevelPlay() {
	if (!GameManager::get()->getGameVariable("0016")) GameManager::get()->setGameVariable("0016", true);
	bool orig83 = GameManager::get()->getGameVariable("0083");
	GameManager::get()->setGameVariable("0083", true);
	if (!m_fields->m_startedAutoDownload && Settings::shouldDownloadSoundsOnLevelPlay() && m_downloadBtn && m_downloadBtn->isVisible()) {
		m_fields->m_startedAutoDownload = true;
		m_downloadBtn->activate();
	}
	GameManager::get()->setGameVariable("0083", orig83);
}

void AutoDownloadCustomSongWidget::tryPlayIfInLevelInfo() {
	/*
	auto director = cocos2d::CCDirector::sharedDirector();
	auto scene = director->getRunningScene();
	if (!scene) return;
	if (!m_fields->m_loadingPopupShown) return;

	bool loadingPopupShown = false;
	for (auto node : CCArrayExt<cocos2d::CCNode*>(scene->getChildren())) {
		if (auto alert = typeinfo_cast<FLAlertLayer*>(node)) {
			loadingPopupShown = true;
			m_fields->m_loadingPopupShown = false;
			alert->keyBackClicked();

			CCNode* node = this;
			while (node) {
				if (auto levelInfo = typeinfo_cast<LevelInfoLayer*>(node)) {
					levelInfo->onPlay(nullptr);
					break;
				}
				node = node->getParent();
			}

			return;
		}
	}
	*/
	auto scene = CCScene::get();
	if (!scene) return;

	auto levelInfo = scene->getChildByType<LevelInfoLayer>(0);
	if (!levelInfo) return;

	levelInfo->onPlay(nullptr);
}

void AutoDownloadCustomSongWidget::allAudiosDownloaded() {
	if (!Settings::shouldAutoPlayOnDownloadFinish()) return;
	tryPlayIfInLevelInfo();
}

/*
void AutoDownloadCustomSongWidget::downloadSongFinished(int id) {
	CustomSongWidget::downloadSongFinished(id);

	auto undownloadedAssetCount = m_undownloadedAssets.size();
	if (undownloadedAssetCount == 0 || (undownloadedAssetCount == 1 && m_undownloadedAssets.at(0).m_id == id)) {
		allAudiosDownloaded();
	}
}
void AutoDownloadCustomSongWidget::downloadSFXFinished(int id) {
	CustomSongWidget::downloadSFXFinished(id);

	auto undownloadedAssetCount = m_undownloadedAssets.size();
	if (undownloadedAssetCount == 0 || (undownloadedAssetCount == 1 && m_undownloadedAssets.at(0).m_id == id)) {
		allAudiosDownloaded();
	}
}
*/

void AutoDownloadCustomSongWidget::showError(bool p0) {
	CustomSongWidget::showError(p0);

	auto levelInfoLayer = CCScene::get()->getChildByType<LevelInfoLayer>(0);
	if (!levelInfoLayer || !levelInfoLayer->m_songWidget || this != levelInfoLayer->m_songWidget) return;

	if (!m_errorLabel->isVisible() || utils::string::toLower(m_errorLabel->getString()) != "download complete.") return;
	if (m_errorLabel->getColor().r != 0 || m_errorLabel->getColor().b != 0 || m_errorLabel->getColor().g != 255) return;

	auto undownloadedAssetCount = m_undownloadedAssets.size();
	if (undownloadedAssetCount == 0 || (undownloadedAssetCount == 1 && m_undownloadedAssets.at(0).m_id == m_songInfoObject.m_songID)) {
		allAudiosDownloaded();
	}
}