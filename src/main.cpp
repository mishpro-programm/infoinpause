#include <Geode/Geode.hpp>

using namespace geode::prelude;

#include <Geode/modify/PauseLayer.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/CCLayer.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>
class $modify(MyPauseLayer, PauseLayer) {
	void customSetup(){
		PauseLayer::customSetup();
		auto menu = this->getChildByID("right-button-menu");
		auto sprite = CCSprite::createWithSpriteFrameName("GJ_chatBtn_001.png");
		sprite->setScale(0.65f);
		auto btn = CCMenuItemSpriteExtra::create(sprite, this, menu_selector(MyPauseLayer::onLevelInfo));
		menu->setLayout(ColumnLayout::create()->setAxisAlignment(AxisAlignment::End));
		menu->addChild(btn);
		btn->setID("level-info-button");
		menu->updateLayout();
	}
	void onLevelInfo(CCObject* sender){
		auto playLayer = GameManager::get()->getPlayLayer();
		if(!playLayer){
			FLAlertLayer::create("Error", "Could not get PlayLayer", "OK")->show();
			return;
		}
		auto level = playLayer->m_level;
		auto infoLayer = InfoLayer::create(level, nullptr, nullptr);
		infoLayer->setZOrder(11);
#ifndef GEODE_IS_MACOS
		infoLayer->setScale(0.f);
		auto scaleAction = CCScaleTo::create(0.3f, 1.f);
		auto action = CCEaseBackOut::create(scaleAction);
		infoLayer->runAction(action);
#endif
		auto scene = CCDirector::sharedDirector()->getRunningScene();
		scene->addChild(infoLayer);
	}
};
//i copied this code from globed (tysm dankmeme)
class $modify(FixedPlayLayer, PlayLayer) {
	bool isCurrentPlayLayer(){
		auto playLayer = geode::cocos::getChildOfType<PlayLayer>(cocos2d::CCScene::get(), 0);
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
class $modify(LevelInfoLayer) {
	void onPlay(CCObject* s){
		if(PlayLayer::get()){
			FLAlertLayer::create("Error", "Can't open level while already in another level", "OK")->show();
			return;
		}
		LevelInfoLayer::onPlay(s);
	}
	void tryCloneLevel(CCObject* s){
		if(PlayLayer::get()){
			FLAlertLayer::create("Error", "Can't clone level while already in another level", "OK")->show();
			return;
		}
		LevelInfoLayer::tryCloneLevel(s);
	}
};
