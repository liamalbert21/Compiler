#pragma once

#include <filesystem>

const inline std::filesystem::path ROOT_DIR{ PROJECT_ROOT_DIR };

namespace Settings {
    namespace File {
        inline std::filesystem::path input{ "data/Code.txt" };
        inline std::filesystem::path output{ "data/Results.txt" };
    }

    namespace Text {
        inline std::size_t right_just{ 30 };
    }

    namespace Limits {
        inline std::size_t max_errors{ 10 };
    }
}