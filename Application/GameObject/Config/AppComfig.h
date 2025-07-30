#pragma once



// オブジェクトの属性
namespace Attributes {

    enum class Category {
        NONE,
        PLAYER,
        ENEMY,
        TERRAIN,
        OTHER,
    };

    enum class Type {
        NONE,
        BODY,       // プレイヤー本体
        BULLET,     // プレイヤーの弾
        SOLID,      // 固い地形（障害物）
        PASSABLE,   // 通れる地形
        SWEERER,    // スイーパー
        OTHER       // その他
    };
}
