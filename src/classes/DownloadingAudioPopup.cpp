#include "DownloadingAudioPopup.hpp"

bool DownloadingAudioPopup::setup(std::function<void(bool)> onChoice,
                               std::function<void()> onClosed) {
    m_onChoice = std::move(onChoice);
    m_onClosed = std::move(onClosed);

    this->setTitle("DOWNLOADING AUDIO");
    this->setContentSize({ 360.f, 170.f });

    auto label = CCLabelBMFont::create(
        "Please wait for audio assets to download...",
        "bigFont.fnt"
    );
    label->setScale(0.45f);
    label->setAnchorPoint({ 0.5f, 0.5f });
    label->setPosition({ this->getContentWidth() / 2.f, 95.f });
    this->m_mainLayer->addChild(label);

    auto backBtn = CCMenuItemSpriteExtra::create(
        ButtonSprite::create("Back"),
        this,
        menu_selector(DownloadingAudioPopup::onBack)
    );

    auto skipBtn = CCMenuItemSpriteExtra::create(
        ButtonSprite::create("Skip"),
        this,
        menu_selector(DownloadingAudioPopup::onSkip)
    );

    auto menu = CCMenu::create();
    menu->addChild(backBtn);
    menu->addChild(skipBtn);
    menu->alignItemsHorizontallyWithPadding(20.f);
    menu->setPosition({ this->getContentWidth() / 2.f, 45.f });
    this->m_mainLayer->addChild(menu);

    return true;
}

void DownloadingAudioPopup::onClose(CCObject* sender) {
    Popup::onClose(sender);
    if (m_onClosed)
        m_onClosed();
}

void DownloadingAudioPopup::onBack(CCObject*) {
    if (m_onChoice)
        m_onChoice(false);
    this->onClose(nullptr);
}

void DownloadingAudioPopup::onSkip(CCObject*) {
    if (m_onChoice)
        m_onChoice(true);
    this->onClose(nullptr);
}

DownloadingAudioPopup* DownloadingAudioPopup::create(
    std::function<void(bool)> onChoice,
    std::function<void()> onClosed
) {
    auto p = new DownloadingAudioPopup();
    if (p && p->init(1)) {
        p->autorelease();
        return p;
    }
    CC_SAFE_DELETE(p);
    return nullptr;
}
