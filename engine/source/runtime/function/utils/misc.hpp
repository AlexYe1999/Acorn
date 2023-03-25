#pragma once

#include <fstream>
#include <vector>

namespace Acorn
{
    static std::vector<char> ReadFromFile(char const* path)
    {
		std::ifstream file(path, std::ios::ate | std::ios::binary);

		if (!file.is_open())
		{
			throw std::runtime_error("failed to open file!");
		}

		size_t file_size = static_cast<size_t>(file.tellg());
		std::vector<char> buffer(file_size);

		file.seekg(0);
		file.read(buffer.data(), file_size);
		file.close();
		
		return buffer;
    }
}