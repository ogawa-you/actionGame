#pragma once

// タイトルシーン
class Goal : public App::Scene
{
public:

	Goal(const InitData& init);

	void update() override;

	void draw() const override;

	//画像
	Texture goalImg{ U"image/goal.png" };

	//音源
	Audio goalAudio{ U"sound/goal.mp3" };
private:
};
