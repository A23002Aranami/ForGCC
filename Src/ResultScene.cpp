#include "ResultScene.h"
#include "ResultScore.h"
ResultScene::ResultScene()
{
	ResultScore* sc = Instantiate<ResultScore>();
}

ResultScene::~ResultScene()
{
}

void ResultScene::Update()
{
}

void ResultScene::Draw()
{
	GameDevice()->m_pFont->Draw(
		0, 20, "ResultScene", 16, RGB(255, 0, 0));
}
