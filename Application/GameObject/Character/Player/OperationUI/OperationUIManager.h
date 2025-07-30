#pragma once

#include "Interface/IOperationUI.h"
#include "ContFrame/OperationUI_ContFrame.h"
#include "Line/OperationUI_Line.h"
#include "LStick/OperationUI_LStick.h"
#include "RB/OperationUI_RB.h"
#include "RStick/OperationUI_RStick.h"


/* 操作UIマネージャー */
class OperationUIManager {

public:

    enum class OperationUIType {
        ContFrame,
        Line,
        LStick,
        RB,
        RStick,
        Count,
    };

public:

    /// <summary>
    /// コンストラクタ
    /// </summary>
    OperationUIManager() = default;

    /// <summary>
    /// デストラクタ
    /// </summary>
    ~OperationUIManager() = default;

    /// <summary>
    /// 初期化処理
    /// </summary>
    void Init();

    /// <summary>
    /// 更新処理
    /// </summary>
    void Update();

    /// <summary>
    /// 描画処理
    /// </summary>
    void Draw2DFront();


private:

    std::vector<std::unique_ptr<IOperationUI>> opUIs_;

    // Inputクラス
    Input *input_ = nullptr;
    
	// デッドゾーン
    const float DZone_ = 0.2f;

    // Stickの入力を取得
    Vector2 iLStick_{};
    Vector2 iRStick_{};

};
