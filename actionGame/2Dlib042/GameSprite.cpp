# include <Siv3D.hpp>
# include "GameSprite.h"

// 2019 7/09
// スタティック変数の宣言

// Stopwatch GameSprite::timer;
// Point GameSprite::scrollPosition;

// 画面更新を60にする	//OpenSiv3Dの仕様変更により使えなくなりました
// 何もしません

/// @brief 画面更新数を決める	//OpenSiv3Dの仕様変更により使えなくなりました ※何もしません
/// @param fps FPS数
void SetFps(int fps )
{
	//if (static_cast<int>(Graphics::GetDisplayRefreshRateHz()) != fps && fps>0 )
	//{
	//	Graphics::SetTargetFrameRateHz(fps);
	//}
	// 0.6.X 以降 命令削除された
	// _ASSERT_EXPR(false,L"Graphics::GetDisplayRefreshRateHz() が削除されて使えなくなりました");
}

// 画面を1280＊720にして、画面更新をFPS60にして、スクロール座標を初期化
//void Start2Dlib(bool fullscreen,int fps,Size resolution)
//{
//	Window::Resize(resolution);
//	SetFps(fps);
//	// Window::SetFullscreen(fullscreen,resolution); // 命令仕様変更によるエラー
//	Window::SetFullscreen(fullscreen);
//	GameSprite::SetScrollPosition();
//}
// 画面を1280＊720にして、画面更新をFPS60にして、スクロール座標を初期化
void ScreenInit(bool fullscreen,int fps,Size resolution)
{
	Window::Resize(resolution);
	SetFps(fps);
	if (fullscreen)
	{
		Window::SetFullscreen(fullscreen);// 重いので不要ならやらない
	}
	Scene::Resize(resolution);
	GameSprite::SetScrollPosition();
}
