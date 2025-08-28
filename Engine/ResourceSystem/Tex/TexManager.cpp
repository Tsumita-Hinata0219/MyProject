#include "TexManager.h"



void TexManager::Init()
{

}

void TexManager::Finalize()
{


}

void TexManager::LoadTexture(const std::string& path, const std::string& name)
{
	// 既に読み込んでいれば早期return
	if(Check_LoadedData(name)) return;

	// フルパスの作成
	std::string fullPath = "Resources/" + path + "/" + name;
	// 拡張子の抽出
	std::string format = GetExtension(name);

	if (format == TexFormat::PNG.first || format == TexFormat::JPEG.first)
	{
		Create_TexData(fullPath);
	}
	else if (format == TexFormat::DDS.first) 
	{

	}
}

bool TexManager::Check_LoadedData(const std::string& key)
{
	auto data = texMaps_.find(key);

	if (data != texMaps_.end()) {
		return true;
	}
	return false;
}

void TexManager::Create_TexData(const std::string& fullpath)
{
	// Textureを読んで転送する
	DirectX::ScratchImage mipImages = Create_MipImage(fullpath, TexFormat::PNG.second);
	const DirectX::TexMetadata& metadata = mipImages.GetMetadata();

}

DirectX::ScratchImage TexManager::Create_MipImage(const std::string& fullPath, const uint32_t format)
{
	DirectX::ScratchImage image{};
	std::wstring filePathw{};
	DirectX::ScratchImage mipImages{};

	// テクスチャファイルを読み込んでプログラムで扱えるようにする
	if (format == TexFormat::PNG.second || format == TexFormat::JPEG.second)
	{
		// Textureデータを読み込む
		filePathw = ConverString(fullPath);
		HRESULT hr = DirectX::LoadFromWICFile(filePathw.c_str(), DirectX::WIC_FLAGS_FORCE_SRGB, nullptr, image);
		assert(SUCCEEDED(hr));

		// ミニマップの作成
		hr = DirectX::GenerateMipMaps(
			image.GetImages(), image.GetImageCount(), image.GetMetadata(),
			DirectX::TEX_FILTER_SRGB, 0, mipImages);
		assert(SUCCEEDED(hr));
	}
	else if (format == TexFormat::DDS.second)
	{
		// Textureデータを読み込む
		filePathw = ConverString(fullPath);
		HRESULT hr = DirectX::LoadFromDDSFile(filePathw.c_str(), DirectX::DDS_FLAGS_NONE, nullptr, image);
		assert(SUCCEEDED(hr));

		// ミニマップの作成
		if (DirectX::IsCompressed(image.GetMetadata().format)) { // 圧縮フォーマットかどうかを調べる
			mipImages = std::move(image); // 圧縮フォーマットならそのまま使うのでstd::moveする
		}
		else {
			hr = DirectX::GenerateMipMaps(
				image.GetImages(), image.GetImageCount(), image.GetMetadata(),
				DirectX::TEX_FILTER_SRGB, 4, mipImages);
			assert(SUCCEEDED(hr));
		}
	}

	return mipImages;
}
