#pragma once

#include <Geode/Geode.hpp>
#include <Geode/ui/Popup.hpp>

using namespace geode::prelude;

class DownloadingAudioPopup
    : public Popup<std::function<void(bool)>, std::function<void()>> {
public:
    static DownloadingAudioPopup* create(
        std::function<void(bool)> onChoice,
        std::function<void()> onClosed
    );

protected:
    bool setup(std::function<void(bool)> onChoice,
               std::function<void()> onClosed) override;

    void onClose(CCObject* sender) override;

private:
    void onBack(CCObject*);
    void onSkip(CCObject*);

    std::function<void(bool)> m_onChoice;
    std::function<void()> m_onClosed;
};
