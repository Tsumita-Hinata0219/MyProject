#include "IActor.h"
#include "../Component/Base/IBaseComponent.h"
#include "../Component/Render/IRenderComponent.h"
#include "../Manager/GameEntityManager.h"


/// <summary>
/// コンストラクタ
/// </summary>
IActor::IActor()
{
	// ステートはActiveに
	state_ = State::Active;

	// 名前はdefaultで初期化
	name_ = "default";
}


/// <summary>
/// デストラクタ
/// </summary>
IActor::~IActor()
{
	// ComponentMapのClear
	componentMap_.clear();
}


/// <summary>
/// 生成
/// </summary>
void IActor::Create(std::string name)
{
	// 名前の設定
	name_ = name;

	// TransformNodeの作成
	transNode_ = std::make_shared<TransformNode>();
}


/// <summary>
/// 更新処理関係
/// </summary>
void IActor::Update(float deltaTime)
{
	// ステートアクティブ時、更新
	if (state_ == State::Active) {
		UpdateActor(deltaTime);
		UpdateComponents(deltaTime);
	}
	transNode_->Update(); // TransformNodeの更新
}
void IActor::UpdateComponents([[maybe_unused]] float deltaTime) 
{
	for (auto& component : componentMap_) {
		component.second->Update(deltaTime);
	}
	if (isRender_) {
		renderComponent_->Update();
	}
}


/// <summary>
/// 描画処理
/// </summary>
void IActor::Render()
{
	if (isRender_) {
		renderComponent_->Draw();
	}
}


/// <summary>
/// コンポーネントの追加
/// </summary>
void IActor::AddComponent(std::shared_ptr<IBaseComponent> component)
{
	// 名前で検索をかける
	auto it = componentMap_.find(component->Get_Name());
	// 既に存在していたら早期return
	if (it != componentMap_.end())
	{
		return;
	}

	std::string name = component->Get_Name();
	auto& newComponent = component;

	newComponent->SetOwner(GetSharedPtr()); // Ownerの設定
	newComponent->Init();                   // 初期化処理
	componentMap_[name] = newComponent;     // Mapに追加
}


/// <summary>
/// Renderコンポーネントの追加
/// </summary>
void IActor::AddComponent(std::shared_ptr<IRenderComponent> component)
{
	// 既に存在していたら早期return
	if (renderComponent_) {
		return;
	}

	auto& newComponent = component;

	newComponent->SetOwner(GetSharedPtr()); // Ownerの設定
	newComponent->Init();                   // 初期化処理
	renderComponent_ = newComponent;
	isRender_ = true;
}
