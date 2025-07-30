local DataA = {
    scale = {x = 1.0, y = 1.0, z = 1.0},
    rotate = {x = 0.0, y = 0.0, z = 0.0},
    translate = {x = 1.0, y = 2.0, z = 3.0},
}

local DataB = {
    name = "Player",
    HP = 100,
    AttackDamage = 10,
}

local keys = {"DataA", "DataB"}

return {
    DataA = DataA,
    DataB = DataB,
    __keys = keys,
}