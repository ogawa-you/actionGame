#pragma once

// タイトルシーン
class Title : public App::Scene
{
public:

	Title(const InitData& init);

	void update() override;

	void draw() const override;

	//画像
	Texture backTex{ U"image/back.png" };
	Texture titleTex{ U"image/Title.png" };
	Texture methodOfOperation{ U"image/move.png" };
	Texture methodOfOperation2{ U"image/dush.png" };
	Texture batu{ U"image/batu.png" };

	Audio titleBGM{ Audio::Stream, U"sound/ファンタジー-日常-.mp3" , Loop::Yes };

	//メニュー
	Array<String> gameName = { U"・ステージ１",U"・ステージ２",U"・ステージ３",U"・せってい",U"・おわる"};
	Menu gameMenu;

	Array<String> operationExplanationString = {U"↑↓キーでせんたく,ZキーかマウスLでけってい"};
	Menu operationExplanation;

private:
};

