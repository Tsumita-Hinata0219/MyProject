
_G.position = { x = 10.0, y = 0.0, z = 0.0, w = 0.0 } -- 初期位置
_G.angle = 0.0 -- 角度
_G.radius = 10.0 -- 初期半径
_G.base_speed = 0.01 -- 最低回転速度


-- 位置と速度を引数に取る移動処理
function Move(velocity)
    -- 移動処理（位置に速度を加算）
    position.x = position.x + velocity.x
    position.y = position.y + velocity.y
    position.z = position.z + velocity.z
    position.w = 0.0
end


-- 位置と速度を引数に取る移動処理（円運動）
function MoveCircular(velocity)
    -- 角速度を決定（velocity.x が 0 の場合でも動くようにする）
    local angular_speed = math.max(math.abs(velocity.x) * 0.1, _G.base_speed)
    
    -- 角度を更新
    _G.angle = _G.angle + angular_speed

    -- 半径を velocity.z で増減可能にする（螺旋運動）
    _G.radius = _G.radius + velocity.z * 0.1

    -- 新しい位置を計算（XZ 平面で円運動）
    position.x = math.cos(_G.angle) * _G.radius
    position.z = math.sin(_G.angle) * _G.radius

    -- Y軸は通常の加算（上下移動）
    position.y = position.y + velocity.y
    position.w = 0.0
end