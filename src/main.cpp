#include <Geode/modify/PauseLayer.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>
#include <Geode/modify/CCLayer.hpp>
#include <Geode/modify/PlayLayer.hpp>

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
		if (!CCScene::get()->getChildByType<PauseLayer>(0)) return;
		const auto pl = PlayLayer::get();
		if (!pl) return;
		const auto level = pl->m_level;
		if (!level) return;
		const auto infoLayer = InfoLayer::create(level, nullptr, nullptr);
		if (!infoLayer) return;
		infoLayer->show(); // the other two need to be nullptr so robtop knows which InfoLayer to generate
	}
};

class $modify(LevelInfoLayer) {
	void showError(const std::string& s) {
		FLAlertLayer::create("Error", s, "OK")->show();
	}
	void onPlay(CCObject* s) {
		if (PlayLayer::get()) return showError("Can't open level while already in another level");
		LevelInfoLayer::onPlay(s);
	}
	void tryCloneLevel(CCObject* s) {
		if (PlayLayer::get()) return showError("Can't clone level while already in another level");
		LevelInfoLayer::tryCloneLevel(s);
	}
};
class $modify(FixedPlayLayer, PlayLayer) {
	bool isCurrentPlayLayer(){
		auto playLayer = cocos2d::CCScene::get()->getChildByType<PlayLayer>(0);
		return playLayer == this;
	}
	bool isPaused(bool checkCurrent){
		if(checkCurrent && !isCurrentPlayLayer()) return false;

		for(CCNode* child : CCArrayExt<CCNode*>(this->getParent()->getChildren())) {
			if(typeinfo_cast<PauseLayer*>(child)) {
				return true;
			}
		}

		return false;
	}
	void onEnterH(){
		auto weRunningScene = this->getParent() == CCScene::get();

		if(weRunningScene){
			CCLayer::onEnter();
			return;
		}

		Loader::get()->queueInMainThread([self = Ref(this)] {
		        if (!self->isPaused(false)) {
		            self->CCLayer::onEnter();
		        }
		    });
	}
};
class $modify(MyCCLayer, CCLayer){
	void onEnter(){
		if(reinterpret_cast<void*>(PlayLayer::get()) == reinterpret_cast<void*>(this)){
				auto pl = reinterpret_cast<FixedPlayLayer*>(static_cast<CCLayer*>(this));
				pl->onEnterH();
		} else {
			CCLayer::onEnter();
		}
	}
};
