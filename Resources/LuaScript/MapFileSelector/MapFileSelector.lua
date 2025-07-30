function GetStageFileName(stageIndex)
    local fileNames = {
        [0] = "Stage_Demo.json",
        [1] = "Stage_1.json",
        [2] = "Stage_2.json",
        [3] = "Stage_3.json",
        [4] = "Stage_4.json",
        [5] = "Stage_5.json"
    }

    return fileNames[stageIndex] or "Stage_Demo.json"
end
