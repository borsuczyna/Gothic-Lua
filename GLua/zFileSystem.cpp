namespace GOTHIC_ENGINE {
	std::string zFileSystem::GetCurrentDirectory()
	{
		char buffer[MAX_PATH];
		DWORD length = GetModuleFileName(nullptr, buffer, MAX_PATH);
		if (length == 0) return "";
		std::string fullPath(buffer);
		std::string::size_type lastSlashPos = fullPath.find_last_of("\\/");
		if (lastSlashPos == std::string::npos) return "";
		std::string directory = fullPath.substr(0, lastSlashPos);
		return directory + "\\";
	}

	std::vector<std::string> zFileSystem::ScanDirectory(const std::string& path) {
		std::vector<std::string> files;

		for (const auto& entry : std::filesystem::directory_iterator(path)) {
			if (entry.is_regular_file()) {
				files.push_back(entry.path().string());
			}
		}

		return files;
	}

	bool zFileSystem::DirectoryExists(const std::string& path) {
		return std::filesystem::is_directory(std::filesystem::status(path));
	}

	bool zFileSystem::FileExists(const std::string& path) {
		return std::filesystem::is_regular_file(std::filesystem::status(path));
	}

	std::string zFileSystem::Extension(const std::string& path) {
		size_t dotPos = path.find_last_of('.');
		if (dotPos != std::string::npos) {
			return path.substr(dotPos + 1);
		}
		return "";
	}
}