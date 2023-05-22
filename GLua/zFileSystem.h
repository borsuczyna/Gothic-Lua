namespace GOTHIC_ENGINE {
	class zFileSystem {
	public:
		static std::string GetCurrentDirectory();
		static std::vector<std::string> ScanDirectory(const std::string& path);
		static bool DirectoryExists(const std::string& path);
		static bool FileExists(const std::string& path);
		static std::string Extension(const std::string& path);
	};
}