#pragma once

#include <iostream>
#include <cstdint>
#include <string>
#include <format>
#include <cassert>
#include <wrl.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <cassert>
#include "imgui.h"
#include <numbers>
#include <type_traits>
#include <filesystem>

#include "Math/MyMath.h"
#include "Physics/CollisionSystem/Structure/CollisionStructures.h"


/// -------------------------------------------------------------------------
/// char , std::string
/// -------------------------------------------------------------------------
// string->wstring
std::wstring ConverString(const std::string& str);
// wstring->string
std::string ConvertString(const std::wstring& str);
// wstring->string
void Log(const std::string& message);
// ファイルパスから拡張子を抽出する関数
std::string GetExtension(const std::string& path);
// ファイルパスからファイル名を抽出する関数
std::string RemoveNameSuffix(const std::string& filePath);
// 指定されたディレクトリ内のファイル名を取得
std::vector<std::string> GetFileNamesInDirectory(const std::string& directoryPath);
// 指定されたディレクトリ内のサブディレクトリ名を取得
std::vector<std::string> GetSubdirectories(const std::string& directoryPath);
// 特定の拡張子を持ったファイル名だけを取り出す
std::string FilterFileByExtension(const std::vector<std::string>& filenames, const std::string& extension);
// 指定された拡張子を持つ最初のファイル名を返す関数
std::string FindFirstFileWithExtension(const std::string& directoryPath, const std::string& extension);



/// -------------------------------------------------------------------------
/// Bit
/// -------------------------------------------------------------------------
// ビット分割関数
uint32_t BitSeparate32(uint32_t n);
// モートン番号を算出する関数
uint32_t Get2DMortonNumber(uint32_t x, uint32_t y);
// ビット列から最上位ビットの位置を取得する関数
uint32_t findHighestBitPosition(int bitmask);


/// -------------------------------------------------------------------------
/// float
/// -------------------------------------------------------------------------
// ラープ
float Lerp(const float& start, const float& end, float t);
// クランプ
float Clamp(const float& value, const float& minValue, const float& maxValue);
// 0に近づくほど1になり、1や-1になるほど0を返す関数
float APOneAsZeroCloser(float value);
// 角度を度からラジアンに変換する処理
float ToRadians(float degrees);
// 範囲に変換
float ConvertToRange(Vector2 input, Vector2 output, float value);



/// -------------------------------------------------------------------------
/// 2次元ベクトル
/// -------------------------------------------------------------------------
// 内積
float Dot(const Vector2& v1, const Vector2& v2);
// 外積
float Cross(const Vector2& v1, const Vector2& v2);
// 長さ
float Length(const Vector2& v);
// 絶対値
Vector2 Absolute(const Vector2& v);
// 正規化
Vector2 Normalize(const Vector2& v);
// 正射影ベクトル
Vector2 Project(const Vector2& v1, const Vector2& v2);
// 線形補間
Vector2 Lerp(const Vector2& start, const Vector2& end, const float t);
// Vector3 -> Vector2 への変換
//Vector2 ConvertVector(const Vector3& v, const ViewProjection& view);
// クランプ
Vector2 Clamp(const Vector2& value, const Vector2& minValue, const Vector2& maxValue);


/// -------------------------------------------------------------------------
/// 3次元ベクトル
/// -------------------------------------------------------------------------
// 内積
float Dot(const Vector3& v1, const Vector3& v2);
// 外積
Vector3 Cross(const Vector3& v1, const Vector3& v2);
// 長さ
float Length(const Vector3& v);
// 絶対値
Vector3 Absolute(const Vector3& v);
// 正規化
Vector3 Normalize(const Vector3& v);
// 正射影ベクトル
Vector3 Project(const Vector3& v1, const Vector3& v2);
// 線形補間
Vector3 Lerp(const Vector3& start, const Vector3& end, const float t);
// 球面線形補間
Vector3 SLerp(const Vector3& start, const Vector3& end, const float t);
// 最近接線
//Vector3 ClosestPoint(const Vector3& p, const Segment& s);
// 法線ベクトル
Vector3 Perpendicular(const Vector3& v);
// 座標変換
Vector3 TransformByMatrix(const Vector3 v, const Matrix4x4 m);
// Y軸周りに回転させる関数
Vector3 YawRotation(const Vector3& vec, float angle);
// ベクトル変換
Vector3 TransformNormal(const Vector3& vec, const Vector3& rotation);
Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m);
// Vector2 -> Vector3 への変換
//Vector3 ConvertVector(const Vector2& v);
// Vector2をそのままVector3に入れる
Vector3 CreateVector3FromVector2(const Vector2& v);
// CatmullRom補間
Vector3 CatmullRomInterpolation(const Vector3& p0, const Vector3& p1, const Vector3& p2, const Vector3& p3, float t);
// CatmullRomスプライン曲線上の座標を得る
Vector3 CatmullRomPosition(const std::vector<Vector3>& points, uint32_t index, float t);
// Vector3にアフィン変換と透視補正を適用する
Vector3 TransformWithPerspective(const Vector3& v, const Matrix4x4& m);
// 角度を 0～2π の範囲に正規化
float NormalizeAngle(float angle);
// 最短回転角度を求める
float ShortestAngle(float currentAngle, float targetAngle);


/// -------------------------------------------------------------------------
/// 3x3行列
/// -------------------------------------------------------------------------




/// -------------------------------------------------------------------------
/// 4x4行列
/// -------------------------------------------------------------------------
// 逆行列
Matrix4x4 Inverse(const Matrix4x4& m);
// 転置行列
Matrix4x4 Transpose(const Matrix4x4& m);
// 平行移動行列
Matrix4x4 MakeTranslateMatrix(const Vector3 translate);
// 拡大縮小行列
Matrix4x4 MakeScaleMatrix(const Vector3 scale);
// 回転行列(X,Y,Z)
Matrix4x4 MakeRotateXMatrix(float radian);
Matrix4x4 MakeRotateYMatrix(float radian);
Matrix4x4 MakeRotateZMatrix(float radian);
// 回転行列(all)
Matrix4x4 MakeRotateXYZMatrix(Vector3 rotate);
// 平行移動行列
Matrix4x4 MakeTranslateMatrix(const Vector3 translate);
// 3次元アフィン変換行列 (W = SRT)
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);
// 透視投影行列
Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip);
// 正射影行列
Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip);
// ビューポート変換行列
Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth);
// 任意軸回転行列
Matrix4x4 MakeRotateAxisAngle(const Vector3& axis, float angle);
Matrix4x4 MakeRotateAxisAngle(const Vector3& axis, float cos, float sin);
// ある方向からある方向への回転
Matrix4x4 DirectionToDirection(const Vector3& from, const Vector3& to);



/// -------------------------------------------------------------------------
/// クォータニオン
/// -------------------------------------------------------------------------
// 内積
float Dot(const Quaternion& q1, const Quaternion& q2);
// 長さ
float Length(const Quaternion v);
// 共役Quaternionを返す
Quaternion Conjugate(const Quaternion& q);
// Quaternionのnormを返す
float Norm(const Quaternion& q);
// 正規化したQuaternionを返す
Quaternion Normalize(const Quaternion& q);
// 逆Quatenionを返す
Quaternion Inverse(const Quaternion& q);
// 球面線形補間
Quaternion Slerp(const Quaternion& q1, const Quaternion& q2, float t);
// 任意軸回転を表すQuaternionの生成
Quaternion MakeRotateAxisAngleQuatenion(const Vector3& axis, float angle);
// ベクトルをQuaternionで回転させた結果のベクトルを求める
Vector3 RotateVector(const Vector3& v, const Quaternion& q);
// Quaternionから回転行列を求める
Matrix4x4 MakeRotateMatrix(const Quaternion& q);
// 3次元アフィン変換行列 (W = SRT)
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Quaternion& rotate, const Vector3& translate);

