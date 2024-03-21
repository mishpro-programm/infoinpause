#include <Geode/Geode.hpp>

using namespace geode::prelude;

#include <Geode/modify/PauseLayer.hpp>
#include <Geode/modify/ProfilePage.hpp>
class $modify(MyPauseLayer, PauseLayer) {
	void customSetup(){
		PauseLayer::customSetup();
		auto menu = this->getChildByID("right-button-menu");
		auto sprite = CCSprite::createWithSpriteFrameName("GJ_chatBtn_001.png");
		sprite->setScale(0.65f);
		//auto sprite = ButtonSprite::create("Info");
		auto btn = CCMenuItemSpriteExtra::create(sprite, this, menu_selector(MyPauseLayer::onLevelInfo));
		menu->setLayout(ColumnLayout::create()->setAxisAlignment(AxisAlignment::End));
		menu->addChild(btn);
		btn->setID("level-info-button");
		//btn->setScale(0.675f, 0.675f);
		menu->updateLayout();
	}
	void onLevelInfo(CCObject* sender){
		auto playLayer = GameManager::get()->getPlayLayer();
		if(!playLayer){
			FLAlertLayer::create("Error", "Could not get PlayLayer", "OK")->show();
			return;
		}
		auto level = playLayer->m_level;
		log::error("userid: {}, accountid: {}", level->m_userID, level->m_accountID);
		auto infoLayer = InfoLayer::create(level, nullptr, nullptr);
		infoLayer->setZOrder(11);
		infoLayer->setScale(0.f);
		auto scaleAction = CCScaleTo::create(1.f, 1.f);
		auto action = CCEaseElasticOut::create(scaleAction);
		infoLayer->runAction(action);
		auto scene = CCDirector::get()->getRunningScene();
		scene->addChild(infoLayer);
	}
};
class $modify(ProfilePage) {
	void onMyLevels(CCObject* s){
		if(!PlayLayer::get()){
			ProfilePage::onMyLevels(s);
		} else {
			FLAlertLayer::create("Error", "Sorry, but now you're not able to view player levels while playing level", "OK")->show();
		}
	}
	void onMyLists(CCObject* s){
		if(!PlayLayer::get()){
			ProfilePage::onMyLists(s);
		} else {
			FLAlertLayer::create("Error", "Sorry, but now you're not able to view player lists while playing level", "OK")->show();
		}
	}
};
