SphereEmitter = {
    translate = { x = 1.0, y = 2.0, z = 30.0, w = 40.0 },
    radius = { x = 0.0, y = 0.0, z = 0.0, w = 0.0 },
}

EmitterRange = {
    scaleMin = { x = 0.3, y = 0.3, z = 0.3, w = 0.0 },

    scaleMax = { x = 0.5, y = 0.5, z = 0.5, w = 0.0 },

    rotateMin = { x = 0.0, y = 0.0, z = 0.0, w = 0.0 },
    rotateMax = { x = 0.0, y = 0.0, z = 0.0, w = 0.0 },

    colorMin = { x = 0.0, y = 0.0, z = 0.0, w = 1.0 },
    colorMax = { x = 1.0, y = 1.0, z = 1.0, w = 1.0 },

    velocityMin = { x = -0.1, y = 0.0, z = -0.0, w = 0.0 },
    velocityMax = { x = 0.1, y = 0.1, z = 0.1, w = 0.0 },

    baseLifeTime = 1.0 * 60.0,
    lifeTimeMin = 0.0,
    lifeTimeMax = 1.0 * 60.0,
}

EmitConfig = {
    spawnInterval = 0.5,
    elapsedTime = 0.5,
    spawnCount = 100,
    isEmitting = false,
}

ConstantField = {
    acceleration = { x = 0.0, y = 0.0, z = 0.0 },
    damping = 1.0,
    angularVelocity = { x = 0.0, y = 0.0, z = 0.0 },
    drag = 0.0,
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