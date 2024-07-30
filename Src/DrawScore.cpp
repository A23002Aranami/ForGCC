#include "DrawScore.h"
#include "Score.h"

ScoreDraw::ScoreDraw()
{
}

ScoreDraw::~ScoreDraw()
{
}

void ScoreDraw::Update()
{
}

void ScoreDraw::Draw()
{
	Score* sc = ObjectManager::FindGameObject<Score>();

	char str[64];//•¶Žš—ñ
	sprintf_s<64>(str, "SCORE: %06d",sc->GetScore());//printf‚ÌŒ`‚Å•¶Žš—ñ‚É“ü‚ê‚Ä‚­‚ê‚é

	GameDevice()->m_pFont->Draw(
		600, 20, str, 32, RGB(255, 255, 255));

}

void ScoreDraw::GetScore()
{
	}
