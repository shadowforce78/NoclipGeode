#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include <Geode/ui/GeodeUI.hpp>

using namespace geode::prelude;

// Mod::get()->getSettingValue<bool>("enabled")
// PlayLayer::destroyPlayer(p0, p1)

class $modify(PlayLayer) {

    struct Fields{
        GameObject* antiCheatObject;
        bool initedDeath = false;
    };

    void destroyPlayer(PlayerObject* p0, GameObject* p1) {

        bool noclip_disabled = !Mod::get()->getSettingValue<bool>("enabled");

        if (!m_fields->initedDeath) {
            #if !defined(GEODE_IS_ANDROID64) && !defined(GEODE_IS_MACOS) && !defined(GEODE_IS_IOS)
            if (m_fields->antiCheatObject == nullptr && p1 != nullptr) { // thank you whamer100
                m_fields->antiCheatObject = p1;
                m_fields->initedDeath = true;
            } else if (!m_fields->initedDeath && m_fields->antiCheatObject == nullptr) {
                m_fields->antiCheatObject = p1;
                m_fields->initedDeath = true;
            }
            #else //sorry android64 users, someone has to implement fields for GameObject, because I get errors 
                if (!m_fields->initedDeath && m_fields->antiCheatObject == nullptr) {
                    m_fields->antiCheatObject = p1;
                    m_fields->initedDeath = true;
                }
            #endif
        }

        if(noclip_disabled) {
            PlayLayer::destroyPlayer(p0, p1);
            return;
        }

        if (p1 == m_fields->antiCheatObject) { // nice AC robert
            return PlayLayer::destroyPlayer(p0, p1);
        }
    };
};

class $modify(OpenSettings, PauseLayer)
{
    void btnSettings(CCObject *)
    {
        geode::openSettingsPopup(Mod::get());
    };

    void customSetup()
    {
        // bool shortcut = Mod::get()->getSettingValue<bool>("shortcut");
		bool shortcut = true;
        auto winSize = CCDirector::sharedDirector()->getWinSize();

        auto bottomRightPos =
            ccp((winSize.width / 2) - 41, (winSize.height / 2) - 181);

        PauseLayer::customSetup();
        auto btnSprite = CCSprite::create("GJ_likeBtn_001.png");
        auto menu = this->getChildByID("right-button-menu");
        auto btn = CCMenuItemSpriteExtra::create(
            btnSprite, this, menu_selector(OpenSettings::btnSettings));
        btn->setID("settings-button"_spr);
        btn->setZOrder(10);

        if (shortcut == true)
        {
            menu->addChild(btn);
            menu->updateLayout();
        }
    };
};