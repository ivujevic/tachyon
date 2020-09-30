#pragma once

#include "../../Database/Source/Database.hpp"
#include "jnipp.h"

struct TachyonJNI {
    const tachyon::Database database;

    ~TachyonJNI() = default;
};
