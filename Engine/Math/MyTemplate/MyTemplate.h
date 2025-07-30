#pragma once

#include "../MathStructure/Vector/Vector.h"

namespace Temp {

	struct Color final {

        // カラー
        static const Vector4 RED;
        static const Vector4 GREEN;
        static const Vector4 BLUE;
        static const Vector4 YELLOW;
        static const Vector4 CYAN;
        static const Vector4 MAGENTA;
        static const Vector4 WHITE;
        static const Vector4 BLACK;
	};

    struct Direction final {

        // 方向
        static const Vector3 Front;
        static const Vector3 Back;
        static const Vector3 Left;
        static const Vector3 Right;
        static const Vector3 FrontLeft;
        static const Vector3 FrontRight;
        static const Vector3 BackLeft;
        static const Vector3 BackRight;
    };

}