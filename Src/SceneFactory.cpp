#include "SceneFactory.h"
#include <windows.h>
#include <assert.h>
#include "TitleScene.h"
#include "PlayScene.h"
#include "ResultScene.h"
SceneBase* SceneFactory::CreateFirst()
{
	return new TitleScene();
	return nullptr;
}

SceneBase * SceneFactory::Create(const std::string & name)
{
	if (name == "TitleScene") {
		return new TitleScene();
	}
	if (name == "PlayScene") {
		return new PlayScene();
	}
	if (name == "ResultScene")
	{
		return new ResultScene();
	}
	assert(false);
	return nullptr;
}
