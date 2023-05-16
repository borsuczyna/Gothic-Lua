namespace GOTHIC_ENGINE {
	class zUtils {
	public:
		static std::vector<std::string> ScanDirectory(std::string path) {
			std::vector<std::string> files;

			for (const auto& entry : std::filesystem::directory_iterator(path)) {
				if (entry.is_regular_file()) {
					files.push_back(entry.path().string());
				}
			}

			return files;
		}

		static bool DirectoryExists(const std::string& path) {
			printf("check %s\n", path.c_str());
			return std::filesystem::is_directory(path);
		}

		static bool FileExists(const std::string& path) {
			return std::filesystem::is_regular_file(path);
		}
	};
}