SphereEmitter = {
    translate = { x = 0.0, y = 0.0, z = 0.0, w = 0.0 },
    radius = { x = 0.5, y = 0.5, z = 0.5, w = 0.0 },
}

EmitterRange = {
    scaleMin = { x = 0.5, y = 0.5, z = 0.5, w = 0.0 },
    scaleMax = { x = 0.7, y = 0.7, z = 0.7, w = 0.0 },

    rotateMin = { x = 0.0, y = 0.0, z = 0.0, w = 0.0 },
    rotateMax = { x = 4.0, y = 4.0, z = 4.0, w = .0 },

    colorMin = { x = 0.0, y = 0.0, z = 0.0, w = 1.0 },
    colorMax = { x = 1.0, y = 1.0, z = 1.0, w = 1.0 },

    velocityMin = { x = -0.1, y = 0.0, z = -0.1, w = 0.0 },
    velocityMax = { x = 0.1, y = 0.1, z = 0.1, w = 0.0 },

    baseLifeTime = 1.5 * 60.0,
    lifeTimeMin = 0.0,
    lifeTimeMax = 0.0,
}

EmitConfig = {
    spawnInterval = 0.1,
    elapsedTime = 0.1,
    spawnCount = 25,
    isEmitting = false,
}

ConstantField = {
    acceleration = { x = 0.1, y = 0.1, z = 0.1 },
    damping = 1.0,
    angularVelocity = { x = 0.0, y = 0.0, z = 0.0 },
    drag = 0.7,
    force = { x = 0.0, y = 0.0, z = 0.0 },
    mass = 0.0,
    isUse = false,
}


return {
    SphereEmitter = SphereEmitter,
    EmitterRange = EmitterRange,
    EmitConfig = EmitConfig,
    ConstantField = ConstantField
}