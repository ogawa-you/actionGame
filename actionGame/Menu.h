#pragma once

/// @brief UI作成用クラス(要改善)
class Menu
{
public:

	/// @brief 選択できるメニューを作る、色だけ設定も可能
	/// @param _defaultColor 選択してないときの色
	/// @param _currentColor 選択したときの色
	/// @param _startPos 左上の座標
	Menu(Color _defaultColor, Color _currentColor, Vec2 _startPos = {10,10})
	{
		defaultColor = _defaultColor;
		currentColor = _currentColor;

		startPos = _startPos;
	};

	/// @brief 選択できるメニューを作る
	/// @param _startPos 左上の座標
	Menu(Vec2 _startPos = {10,10})
	{
		startPos = _startPos;
	};

	Vec2 startPos;

	int menuID = 0;				//選択中を判断

	int fontSize = 30;			//フォントの大きさ

	//定義(String)

	/// @brief 文字を表示させる際の初期定義
	/// @param _menuChara 表示させたい文字の配列(配列数によって長さが決まる)
	/// @param _startPos 表示させる一文字目の左上の座標
	void StringSet(Array<String> _menuChara, Vec2 _startPos = {10,10})
	{
		//現在の配列を消去
		menuChara.clear();

		//文字列の長さを入力
		menuCharaSize = _menuChara.size();

		//座標入力
		startPos = _startPos;

		//文字を配列に入力
		for (int i = 0; i < menuCharaSize; i++)
		{
			menuChara << _menuChara[i];
		}
	}

	/// @brief 数字(一桁)を表示させる際の初期定義
	/// @param _numberChara 表示させたい数字の配列(配列数によって長さが決まる)
	/// @param _startPos 表示させる一文字目の左上の座標
	void intSet(Array<int> _numberChara, Vec2 _startPos)
	{
		//現在の配列を消去
		numberChara_int.clear();

		//数字列の長さを入力
		menuCharaSize = _numberChara.size();

		//座標入力
		startPos = _startPos;

		//数字を配列に入力
		for (int i = 0; i < menuCharaSize; i++)
		{
			numberChara_int << _numberChara[i];
		}
	}

	/// @brief 数字(小数点以下あり)を表示させる際の初期定義
	/// @param _numberChara 表示させたい数字の配列(配列数によって長さが決まる)
	/// @param _startPos 表示させる一文字目の左上の座標
	void doubleSet(Array<double> _numberChara, Vec2 _startPos)
	{
		//現在の配列を消去
		numberChara_double.clear();

		//数字列の長さを入力
		menuCharaSize = _numberChara.size();

		//座標入力
		startPos = _startPos;

		//数字を配列に入力
		for (int i = 0; i < menuCharaSize; i++)
		{
			numberChara_double << _numberChara[i];
		}
	}
	
	/// @brief メニューを閉じた際の初期化
	void Initialize()
	{
		menuID = 0;
	}

	/// @brief メニューの上下移動とマウスカーソルでの選択
	void Update()
	{
		//コントローラー
		auto controller = XInput(0);
		double stick = 0.5;

		//↑↓入力で選択できる || コントローラーの上下移動
		if (KeyUp.down() || KeyW.down() || controller.buttonUp.down() || controller.leftThumbY > stick)
		{
			menuID -= 1;		
		}
		if (KeyDown.down() || KeyS.down() || controller.buttonDown.down() || controller.leftThumbY < -stick)
		{
			menuID += 1;	
		}

		if (menuID < 0)
		{
			menuID = menuCharaSize - 1;
		}

		if (menuID > menuCharaSize - 1)
		{
			menuID = 0;
		}

		//マウスカーソルが触れたら移動
		for (int i = 0; i < menuCharaSize; i++)
		{
			bool mouseOver = RectF{ startPos.x, startPos.y + ((fontSize * 1.5) * i),menuChara[i].size() * fontSize,fontSize * 1.4 }.mouseOver();
			if (mouseOver) menuID = i;
		}
	}

	/// @brief メニューの上下移動とマウスカーソルでの選択(桁数補正あり)
	void NumberUpdate_int()
	{
		//↑↓入力で選択できる
		if (KeyUp.down() || KeyW.down())
		{
			menuID -= 1;

			if (menuID < 0)
			{
				menuID = menuCharaSize - 1;
			}
		}
		if (KeyDown.down() || KeyS.down())
		{
			menuID += 1;

			if (menuID > menuCharaSize - 1)
			{
				menuID = 0;
			}
		}

		for (int i = 0; i < menuCharaSize; i++)
		{
			//桁数計算
			int number = numberChara_int[i];
			int digit = 0;
			while (number != 0)
			{
				number = number / 10;
				digit++;
			}
			//数値が0だと桁数が0になるので加算
			if (digit == 0)digit = 1;
			//マウスカーソルが触れたら移動
			bool mouseOver = RectF{ startPos.x, startPos.y + ((fontSize * 1.5) * i),digit * (fontSize / 2),fontSize * 1.4 }.mouseOver();
			if (mouseOver) menuID = i;
		}
	}

	/// @brief ほかのメニューと選択を同期させる
	void InterlockingUpdate(Menu _mainMenu)
	{
		menuID = _mainMenu.menuID;
	}

	/// @brief メニューを描画
	/// @param isCurrentDraw 強調表示するか
	void InRectDraw(bool isCurrentDraw = true)const
	{
		//描画
		for (int i = 0; i < menuCharaSize; i++)
		{
			//選択中の文字なら強調表示
			bool current = (menuID == i);
			font(menuChara[i]).draw(startPos.x, startPos.y + ((fontSize * 1.5) * i),isCurrentDraw ?  (current ? currentColor : defaultColor):defaultColor);
		}
	}

	/// @brief menuIDより小さいmenuはdefaultColor、それ以外はcurrentColor
	void TwoWayDraw()const
	{
		//描画
		for (int i = 0; i < menuCharaSize; i++)
		{
			//選択中の文字なら強調表示
			font(menuChara[i]).draw(startPos.x, startPos.y + ((fontSize * 1.2) * i), i >= menuID ? currentColor : defaultColor);
		}
	}

	/// @brief 数字の描画(小数無し)
	void NumberDraw_int(bool isCurrentDraw)const
	{
		//描画
		for (int i = 0; i < menuCharaSize; i++)
		{
			//選択中の文字なら強調表示
			bool current = (menuID == i);
			font(numberChara_int[i]).draw(startPos.x, startPos.y + ((fontSize * 1.5) * i), isCurrentDraw ? (current ? currentColor : defaultColor) : defaultColor);
		}
	}

	/// @brief 数字の描画(小数あり)
	void NumberDraw_double(bool isCurrentDraw)const
	{
		//描画
		for (int i = 0; i < menuCharaSize; i++)
		{
			//選択中の文字なら強調表示
			bool current = (menuID == i);
			font(numberChara_double[i]).draw(startPos.x, startPos.y + ((fontSize * 1.5) * i), isCurrentDraw ? (current ? currentColor : defaultColor) : defaultColor);
		}
	}

	
	/// @brief 現在のmrnuIDを返す(上から何番目かの数字0スタート)
	int IsCurrent()const
	{
		return menuID;
	}

	
	/// @brief 現在マウスオーバーしているか
	bool IsMouseOver()
	{
		return RectF{ startPos.x, startPos.y + ((fontSize * 1.5) * menuID),menuChara[menuID].size() * fontSize,fontSize * 1.4 }.mouseOver();
	}

private:

	//選択してないときの色
	Color defaultColor = { Palette::White };
	//選択したときの色
	Color currentColor = { Palette::Yellow };

	//表示させる文字列の数
	int menuCharaSize = 1;

	//文字列
	Array<String> menuChara;
	//数字列(小数なし)
	Array<int> numberChara_int;
	//数字列(小数あり)
	Array<double> numberChara_double;

	//フォント
	Font font{ fontSize,U"font/kkm_analogtv.ttf"};
};

