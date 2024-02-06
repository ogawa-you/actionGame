#pragma once

class AnimationMake
{
public:
	AnimationMake() {};

	/// @brief アニメーションの情報
	/// @param _texture 画像
	/// @param _patternOfNumber 横と縦のパターン数(0スタート)
	/// @param _startPattern 開始のパターン数
	/// @param _endPattern 終わりのパターン数
	/// @param _motionTime 1モーションの全体時間
	/// @param _imageCorrectionPos 画像補正位置
	/// @param _charaSize アニメーションの当たり判定のサイズ
	/// @param _resize 画像サイズ補正(がばい)
	AnimationMake(Texture _texture, Size _patternOfNumber, int _startPattern, int _endPattern, int _motionTime = 1000, Point _imageCorrectionPos = Point(0, 0), Size _charaSize = {0,0} , double _resize = 1)
	{
		texture = _texture;

		patternOfNumber = _patternOfNumber;

		startPattern = _startPattern;

		endPattern = _endPattern;

		motionTime = _motionTime;

		imageCorrectionPos = _imageCorrectionPos;

		charaSize = _charaSize;

		resize = _resize;

		//横幅と縦幅を求める
		animationPatternSize = texture.size() / (patternOfNumber + Size{1,1});

		//初期化
		AnimationInitialize();
	};

	/// @brief アニメーションの情報(1パターンのみ)
	/// @param _texture ロードする画像
	/// @param _patternOfNumber 横と縦のパターン数(0スタート)
	/// @param _pattern 描画するパターン
	/// @param _imageCorrectionPos 画像補正位置
	/// @param _charaSize アニメーションの当たり判定のサイズ
	/// @param _resize 画像サイズ補正(がばい)
	AnimationMake(Texture _texture, Size _patternOfNumber, int _pattern, Point _imageCorrectionPos = Point(0, 0), Size _charaSize = { 0,0 }, double _resize = 1)
	{
		texture = _texture;

		patternOfNumber = _patternOfNumber;

		startPattern = _pattern;

		endPattern = _pattern;

		motionTime = 0;

		imageCorrectionPos = _imageCorrectionPos;

		charaSize = _charaSize;

		resize = _resize;

		//横幅と縦幅を求める
		animationPatternSize = texture.size();

		//初期化
		AnimationInitialize();
	};

	//アニメーションの画像
	Texture texture;

	//アニメーションが横と縦に何パターンあるか
	Size patternOfNumber;

	//1アニメーションの縦幅と横幅
	Size animationPatternSize;

	//始点
	int startPattern;

	//終点
	int endPattern;

	//現在のパターン
	int currentPattern;

	//全体のモーション時間(ms)
	int motionTime;

	//画像補正位置
	Point imageCorrectionPos;

	//キャラの縦幅と横幅
	Size charaSize;

	//画像大きさ補正の補正値
	double resize;

	///初期化
	void AnimationInitialize()
	{		
		currentPattern = startPattern;
	}

	/// @brief //全体時間より大きくなったらtrue
	/// @param stopwatch アニメーション管理用ストップウォッチ
	/// @return true or false
	bool IsOnePatternAnimation(Stopwatch& stopwatch)
	{
		//全体時間より大きくなったらtrue
		return (stopwatch.ms() > motionTime -10);
	}

	/// @brief //全体時間ms
	/// @return onepatterntime(ms)
	int OnePatternAnimation_ms()
	{
		//全体時間より大きくなったらtrue
		return motionTime;
	}

	/// @brief アニメーションの描画
	/// @param position 描画座標
	/// @param stopwatch 時間管理用ストップウォッチ
	/// @param isMirror 反転フラグ
	void AnimationDraw(Vec2 position,Stopwatch &stopwatch,Color addColor, bool isMirror = false)
	{
		if (motionTime == 0)
		{
			//texture.resized(animationPatternSize * resize).mirrored(isMirror).draw(position - imageCorrectionPos);

			//現在の切り取り位置
			//currentPatternは0スタートなので+1して1スタートにする
			//patternfNumber.xは0スタートなので実際の横幅は+1の値
			int cutPosY = (currentPattern) / (patternOfNumber.x + 1);
			int cutPosX = currentPattern - (cutPosY * (patternOfNumber.x + 1));

			//描画
			texture(cutPosX * animationPatternSize.x, cutPosY * animationPatternSize.y, animationPatternSize).resized(animationPatternSize * resize).mirrored(isMirror).draw(position - imageCorrectionPos, addColor);

			return;
		}

		//1モーション当たりの時間(ms)
		int oneMotionTime = motionTime / (endPattern + 1 - startPattern);

		//現在のパターンに1モーションの時間をかけた値よりも経過時間が多ければ現在のパターン加算、0スタートだと1パターン目がスルーされるため1加算
		if ((currentPattern - startPattern + 1) * oneMotionTime < stopwatch.ms())
		{
			currentPattern++;
		}	
		
		//全体時間より大きくなったら初期化
		if (IsOnePatternAnimation(stopwatch))
		{
			stopwatch.restart();

			AnimationInitialize();
		}

		//現在の切り取り位置
		//currentPatternは0スタートなので+1して1スタートにする
		//patternfNumber.xは0スタートなので実際の横幅は+1の値
		int cutPosY = (currentPattern) / (patternOfNumber.x + 1);
		int cutPosX = currentPattern - (cutPosY * (patternOfNumber.x + 1)) ;
		

		//描画
		texture(cutPosX * animationPatternSize.x, cutPosY * animationPatternSize.y, animationPatternSize).resized(animationPatternSize * resize).mirrored(isMirror).draw(position - imageCorrectionPos,addColor);
	}

private:

};
