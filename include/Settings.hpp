#pragma once

#include <filesystem>

namespace Settings {
    namespace File {
        inline std::filesystem::path root_dir{ PROJECT_ROOT_DIR };
        inline std::filesystem::path input{ "Data/Code.txt" };
        inline std::filesystem::path output{ "Data/Results.txt" };
    }

    namespace Text {
        inline std::size_t right_just{ 30 };
    }
}