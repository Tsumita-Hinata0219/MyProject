#include "OperationUIManager.h"


/// <summary>
/// 初期化処理
/// </summary>
void OperationUIManager::Init() 
{
    input_ = Input::GetInstance();

    const int indexSize = EnumSize<OperationUIType>::value;

    // テクスチャの読み込み
    std::string rootPath = "Texture/Game/Operation_UI";
    uint32_t texHDs[indexSize] = {
        TextureManager::LoadTexture(rootPath, "Cont_Frame.png"),
        TextureManager::LoadTexture(rootPath, "Cont_Line.png"),
        TextureManager::LoadTexture(rootPath, "OP_LStick.png"),
        TextureManager::LoadTexture(rootPath, "OP_RB.png"),
        TextureManager::LoadTexture(rootPath, "OP_RStick.png"),
    };

    // 画像サイズ
    Vector2 spriteSize = {340.0f, 191.25f};
    // 初期座標
    Vector2 initPos = {1020.0f, 600.0f};

    // 配列のサイズ設定
    opUIs_.resize(indexSize);

    // 初期化
    for (int i = 0; i < indexSize; ++i) {
        opUIs_[i] = std::make_unique<IOperationUI>();
        opUIs_[i]->SetTextureHandle(texHDs[i]);
        opUIs_[i]->SetSize(spriteSize);
        opUIs_[i]->SetInitPos(initPos);
        opUIs_[i]->Init();
    }
}


/// <summary>
/// 更新処理
/// </summary>
void OperationUIManager::Update() 
{
    opUIs_[int(OperationUIType::LStick)]->SetColor(Temp::Color::WHITE);
    opUIs_[int(OperationUIType::RB)]->SetColor(Temp::Color::WHITE);
    opUIs_[int(OperationUIType::RStick)]->SetColor(Temp::Color::WHITE);


    /* 入力時は色を変える */

    if (input_->Press(PadData::RIGHT_SHOULDER)) {
        opUIs_[int(OperationUIType::RB)]->SetColor(Temp::Color::YELLOW);
    }
    
    // stickの入力を取得
    iLStick_ = input_->GetLStick();
    iRStick_ = input_->GetRStick();

    // 移動量の計算
    if (std::abs(iLStick_.x) > DZone_ || std::abs(iLStick_.y) > DZone_) {
        opUIs_[int(OperationUIType::LStick)]->SetColor(Temp::Color::YELLOW);
    }
    if (std::abs(iRStick_.x) > DZone_ || std::abs(iRStick_.y) > DZone_) {
        opUIs_[int(OperationUIType::RStick)]->SetColor(Temp::Color::YELLOW);
    }
}


/// <summary>
/// 描画処理
/// </summary>
void OperationUIManager::Draw2DFront() 
{
    for (auto &itr : opUIs_) {
        itr->Draw2DFront();
    }
}
