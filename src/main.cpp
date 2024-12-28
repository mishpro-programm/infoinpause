#include <Geode/modify/PauseLayer.hpp>

using namespace geode::prelude;

class $modify(MyPauseLayer, PauseLayer) {
	void customSetup() {
		PauseLayer::customSetup();
		auto menu = this->getChildByID("right-button-menu");
		if (!menu) return;
		auto sprite = CCSprite::createWithSpriteFrameName("GJ_chatBtn_001.png");
		sprite->setScale(0.65f);
		auto btn = CCMenuItemSpriteExtra::create(sprite, this, menu_selector(MyPauseLayer::onCommentsButton));
		btn->setID("level-info-button"_spr);
		menu->addChild(btn);
		menu->updateLayout();
	}
	void onCommentsButton(CCObject* sender) {
		const auto pl = PlayLayer::get();
		if (!pl) return;
		const auto level = pl->m_level;
		if (!level) return;
		InfoLayer::create(level, nullptr, nullptr)->show(); // the other two need to be nullptr so robtop knows which InfoLayer to generate
	}
};
