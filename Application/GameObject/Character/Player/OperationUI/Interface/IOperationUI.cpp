#include "IOperationUI.h"

/// <summary>
/// 初期化処理
/// </summary>
void IOperationUI::Init() 
{
    // スプライトの初期化
    sprite_ = std::make_unique<Sprite>();
    sprite_->Init(size_);
    sprite_->SetTexture(textureHandle_);
    sprite_->SetAnchor(SpriteAnchor::Center);

    // トランスフォームの初期化
    trans_.Init();
    trans_.srt.translate = {
        initPos_.x,
        initPos_.y,
        0.0f,
    };
}


/// <summary>
/// 更新処理
/// </summary>
void IOperationUI::Update() 
{


}


/// <summary>
/// 描画処理
/// </summary>
void IOperationUI::Draw2DFront() 
{ 
    sprite_->Draw(trans_); 
}
