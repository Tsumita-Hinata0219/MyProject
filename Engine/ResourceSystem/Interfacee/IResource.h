#pragma once

/* 共通resourceの基底クラス */
class IResource {

public:

	/// <summary>
	/// 仮想デストラクタ
	/// </summary>
	virtual ~IResource() = default;
	 
	/// <summary>
	/// 解放処理 : 純粋仮想関数
	/// </summary>
	virtual void Release() = 0;
};