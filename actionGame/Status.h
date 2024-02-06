#pragma once
class Status
{
public:
	Status() {};

	/// @brief 最大HP定義
	/// @param _HP 
	Status(int _HP)
	{
		maxHitpoint = _HP;

		hitpoint = maxHitpoint;
	};

	

	/// @brief 最大HPと最大MP定義
	/// @param _HP 
	/// @param _MP
	/// @param _MPHealPerSec
	/// @param _isMpHeal 
	Status(int _HP, double _MP,double _MPHealPerSec = 0,bool _isMpHeal = true)
	{
		maxHitpoint = _HP;
		maxMagicpoint = _MP;

		magicpointHealPerSec = _MPHealPerSec;
		isMpHeal = _isMpHeal;

		hitpoint = maxHitpoint;
		magicpoint = maxMagicpoint;

		mpHealCount.start();
	};

	/// キャラクター用
	/// @brief 最大HPと最大MPとSTR定義
	/// @param _HP 
	/// @param _MP 
	/// @param _STR
	/// @param _MPHealPerSec 
	/// @param _isMpHeal 
	Status(int _HP,double _MP,int _STR, double _MPHealPerSec = 0, bool _isMpHeal = true)
	{
		maxHitpoint = _HP;
		maxMagicpoint = _MP;
		strength = _STR;

		magicpointHealPerSec = _MPHealPerSec;
		isMpHeal = _isMpHeal;

		hitpoint = maxHitpoint;
		magicpoint = maxMagicpoint;

		mpHealCount.start();
	};

	//最大HP
	int maxHitpoint = 1;

	//最大MP
	double maxMagicpoint = 0;

	//現在HP
	int hitpoint = 1;
	//現在MP
	double magicpoint = 0;
	//攻撃力
	int strength = 0;

	//一秒あたりに自然回復するMP量
	double magicpointHealPerSec = 0;

	//MPの自然回復をするか
	bool isMpHeal = true;

	//MP回復用タイマー
	Stopwatch mpHealCount;

	/// @brief 毎フレーム行う処理
	void Update()
	{
		//0.05秒/毎MP回復
		if (isMpHeal && mpHealCount.ms() > 50)
		{
			//タイマーリセット
			mpHealCount.restart();

			//MP回復
			MpHeal(magicpointHealPerSec * 0.05);
		}	
	}

	/// @brief 最大hp増加
	/// @param addHp 増加したいhp量
	void MaxHpAdd(int addHp)
	{
		maxHitpoint = maxHitpoint + addHp;
	}

	/// @brief 最大mp増加
	/// @param addMp 増加したいmp量
	void MaxMpAdd(double addMp)
	{
		maxMagicpoint = maxMagicpoint + addMp;
	}

	/// @brief hp回復
	/// @param healpoint 回復するhp量
	void HpHeal(int healpoint)
	{
		hitpoint += healpoint;

		//hpが最大hpを超過しないようにする
		if (hitpoint > maxHitpoint)
		{
			hitpoint = maxHitpoint;
		}
	}

	/// @brief mp回復
	/// @param healpoint 回復したいmp量
	void MpHeal(double healpoint)
	{
		magicpoint += healpoint;

		//mpが最大mpを超過しないようにする
		if (magicpoint > maxMagicpoint)
		{
			magicpoint = maxMagicpoint;
		}
	}

	/// @brief HPを最大値まで回復
	void MaxHpHeal()
	{
		hitpoint = maxHitpoint;
	}

	/// @brief MPを最大値まで回復
	void MaxMpHeal()
	{
		magicpoint = maxMagicpoint;
	}

	/// @brief HPを引数分減少
	/// @param _decrease 減少値
	void HpDecrease(int _decrease)
	{
		hitpoint -= _decrease;

		if (hitpoint < 0)
		{
			hitpoint = 0;
		}
	}

	/// @brief MPを引数分減少
	/// @param _decrease 減少値
	void MpDecrease(double _decrease)
	{
		magicpoint -= _decrease;

		if (magicpoint < 0)
		{
			magicpoint = 0;
		}
	}

private:

};

