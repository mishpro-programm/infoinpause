#include <Geode/Geode.hpp>

using namespace geode::prelude;

#include <Geode/modify/PauseLayer.hpp>
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
		auto infoLayer = InfoLayer::create(level, nullptr, nullptr);
		infoLayer->setZOrder(11);
		infoLayer->setScale(0.f);
		auto scaleAction = CCScaleTo::create(0.5f, 1.f);
		auto action = CCEaseElasticOut::create(scaleAction);
		infoLayer->runAction(action);
		auto scene = CCDirector::get()->getRunningScene();
		scene->addChild(infoLayer);
	}
};
