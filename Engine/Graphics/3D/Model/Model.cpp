#include "Model.h"
#include "Manager/ModelManager.h"
#include "Graphics/Animation/KeyFrameAnimation/KeyFrameAnimation.h"
#include "System/Camera/Manager/CameraManager.h"


/// <summary>
/// コンストラクタ
/// </summary>
Model::Model()	
{
	// CameraManagerのインスタンスの取得
	cameraManager_ = CameraManager::GetInstance();
}
Model::Model(ModelDatas datas)
{
	// CameraManagerのインスタンスの取得
	cameraManager_ = CameraManager::GetInstance();

	// フォーマットからステートを決める
	if (datas.fileFormat == ModelFileFormat::OBJ.first) {
		this->modelState_ = new IOBJState(datas);
	}
	else if (datas.fileFormat == ModelFileFormat::GLTF.first) {
		this->modelState_ = new IGLTFState(datas);
	}
}


/// <summary>
/// 描画処理
/// </summary>
void Model::Draw(Transform& transform)
{
	modelState_->Draw(transform);
}

//
///// <summary>
///// Animationの再生
///// </summary>
//void Model::PlayAnimation(Animation animation, float time)
//{
//	// GLTFで読み込んだモデルじゃないとダメ
//	assert(this->state_->GetStateType() == ModelStateType::gLTF);
//
//	// timeにあったAnimationを解析して、LocalMatrixに入れる
//	this->objData_.rootNode.localMatrix = keyFrameAnimation_->PlayAnimation(this->objData_.rootNode.name, animation, time);
//}
//
//
///// <summary>
///// Nodeの階層構造からSkeletonを作る
///// </summary>
//Skeleton Model::CreateSkeleton()
//{
//	// GLTFで読み込んだモデルじゃないとダメ
//	assert(this->state_->GetStateType() == ModelStateType::gLTF);
//
//	return modelManager_->CreateSkeleton(this->objData_.rootNode);
//}


/// <summary>
/// Skeletonの更新処理
/// </summary>
void Model::UpdateSkeleton(Skeleton& skeleton)
{
	// すべてのJointを更新。親が若いので通常ループで処理可能になっている
	for (Joint& joint : skeleton.joints) {

		joint.localMatrix = MakeAffineMatrix(joint.transform.scale, joint.transform.rotate, joint.transform.translate);

		// 親がいれば親の行列を掛ける
		if (joint.parent) {

			joint.skeletonSpaceMatrix = joint.localMatrix * skeleton.joints[*joint.parent].skeletonSpaceMatrix;
		}
		else { // 親がいないのでlocalMatrixとskeletonSpaceMatrixは一致する

			joint.skeletonSpaceMatrix = joint.localMatrix;
		}
	}
}


///// <summary>
///// Animationを適用する
///// </summary>
//void Model::ApplyAnimation(Skeleton& skeleton, const Animation& animation, float animationTime)
//{
//	keyFrameAnimation_->ApplyAnimation(skeleton, animation, animationTime);
//}


///// <summary>
///// SKinClusterの生成
///// </summary>
//SkinCluster Model::CreateSkinCluster(const Skeleton& skeleton)
//{
//	// DescriptorManagerの取得
//	DescriptorManager* descriptor = DescriptorManager::GetInstance();
//
//	// 作成するSkinCluster
//	SkinCluster result{};
//
//	// palette用のResourceを確保
//	result.paletteResource = CreateResource::CreateBufferResource(sizeof(WellForGPU) * skeleton.joints.size());
//	WellForGPU* mappedPalette = nullptr;
//	result.paletteResource->Map(0, nullptr, reinterpret_cast<void**>(&mappedPalette));
//	result.mappedPallette = { mappedPalette, skeleton.joints.size() }; // spanを使ってアクセスするようにする
//	//result.srvHandle = SRVManager::CreateSkinClusterSRV(result.paletteResource, skeleton); // SRVHandleの設定
//	result.srvHandle = descriptor->CreateSkinClusterSRV(result.paletteResource, skeleton); // SRVHandleの設定
//
//	// influence用のResourceを確保
//	result.influenceResource = CreateResource::CreateBufferResource(sizeof(VertexInfluence) * this->objData_.vertices.size());
//	VertexInfluence* mappedInfluence = nullptr;
//	result.influenceResource->Map(0, nullptr, reinterpret_cast<void**>(&mappedInfluence));
//	std::memset(mappedInfluence, 0, sizeof(VertexInfluence) * this->objData_.vertices.size()); // 0埋め。weightを0にしておく
//	result.mappedInfluence = { mappedInfluence, this->objData_.vertices.size() };
//
//	// influence用のVBVを作成
//	result.influenceBufferView.BufferLocation = result.influenceResource->GetGPUVirtualAddress();
//	result.influenceBufferView.SizeInBytes = UINT(sizeof(VertexInfluence) * this->objData_.vertices.size());
//	result.influenceBufferView.StrideInBytes = sizeof(VertexInfluence);
//
//	// InverseBindPoseMatrixの保存領域を作成。単位行列で埋める
//	result.inverseBindPosematrices.resize(skeleton.joints.size());
//	std::generate(result.inverseBindPosematrices.begin(), result.inverseBindPosematrices.end(), []() { return Matrix4x4::identity; });
//
//	// ModelDataのSkinCluster情報を解析してInfluenceの中身を埋める
//	for (const auto& jointWeight : this->objData_.skinClusterData) { // ModelのSkinClusterの情報を解析
//		auto it = skeleton.jointMap.find(jointWeight.first); // jointWeight.firstはjoint名なので、skeletonに対象となるjointが含まれているか判断
//		if (it == skeleton.jointMap.end()) { // そんな名前のjointは存在しない。なので次に回す
//			continue;
//		}
//
//		// (*it).secondにはjointのindexが入っているので、該当indexのinverseBindPoseMatrixを代入
//		result.inverseBindPosematrices[(*it).second] = jointWeight.second.inverseBindPoseMatrix;
//		for (const auto& vertexWeight : jointWeight.second.vertexWeights) {
//			auto& currentInfluence = result.mappedInfluence[vertexWeight.vertexIndex]; // 該当vertexIndexのinfluence情報を参照しておく
//			for (uint32_t index = 0; index < kNumMaxInfluence; ++index) { // 空いているところに入れる
//				if (currentInfluence.weights[index] == 0.0f) { // weight == 0 が開いている状態なので、その場所にweightとjointのindexを代入
//					currentInfluence.weights[index] = vertexWeight.weight;
//					currentInfluence.jointIndices[index] = (*it).second;
//					break;
//				}
//			}
//		}
//	}
//
//	return result;
//}


/// <summary>
/// SkinCLusterの更新処理
/// </summary>
void Model::UpdateSkinCluster(SkinCluster& skinCluster, const Skeleton& skeleton)
{
	for (size_t jointIndex = 0; jointIndex < skeleton.joints.size(); ++jointIndex) {

		assert(jointIndex < skinCluster.inverseBindPosematrices.size());

		skinCluster.mappedPallette[jointIndex].skeletonSpaceMatrix =
			skinCluster.inverseBindPosematrices[jointIndex] * skeleton.joints[jointIndex].skeletonSpaceMatrix;

		skinCluster.mappedPallette[jointIndex].skeletonSpaceInverseTransposeMatrix =
			Transpose(Inverse(skinCluster.mappedPallette[jointIndex].skeletonSpaceMatrix));
	}
}


