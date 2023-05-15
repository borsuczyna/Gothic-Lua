bool consoleOpen = true;
bool scrollToBottom = false;
bool forceFocus = false;
char consoleInput[2048];
const int MAX_WORDS = 1024;

// Log types
struct _LogType {
	int Default = 0;
	int Error = 1;
	int Warning = 2;
	int Input = 3;
} LogType;
ImVec4 outputColors[4] = {};

// Logs
struct LogItem {
	int type;
	std::string message;
};

std::vector<LogItem>& ConsoleLogs() {
	static std::vector<LogItem> logs;
	return logs;
}

// Command Handlers
using CommandCallback = std::function<void(void*)>;
struct CommandHandler {
	char* command;
	CommandCallback callback;
};

std::vector<CommandHandler>& CommandHandlers() {
	static std::vector<CommandHandler> commandHandlers;
	return commandHandlers;
}

// Useful functions
char** splitString(char* input, int& numWords) {
	char** words = new char* [MAX_WORDS];
	numWords = 0;

	char* token = std::strtok(input, " ");
	while (token != nullptr && numWords < MAX_WORDS) {
		words[numWords] = new char[std::strlen(token) + 1];
		std::strcpy(words[numWords], token);
		numWords++;
		token = std::strtok(nullptr, " ");
	}

	return words;
}

void freeMemory(char** words, int numWords) {
	for (int i = 0; i < numWords; i++) {
		delete[] words[i];
	}
	delete[] words;
}

char* toLowerCharArray(char* input) {
	for (int i = 0; i < sizeof(input); i++) {
		input[i] = tolower(input[i]);
	}
	return input;
}

namespace GOTHIC_ENGINE {
	class zConsole {
	private:
		static void CommandExecuted(char* input) {
			int numWords;
			char** arguments = splitString(input, numWords);			
			zEvents::TriggerEvent("onCommandExecuted", &arguments);

			bool commandFound = false;
			for (const auto& commandHandler : CommandHandlers()) {
				if (strcmp(toLowerCharArray(commandHandler.command), toLowerCharArray(arguments[0])) == 0) {
					commandHandler.callback(&arguments);
					commandFound = true;
				}
			}

			if (!commandFound) {
				char message[256];
				sprintf(message, ">> %s", arguments[0]);
				Log(message, LogType.Input);

				sprintf(message, "Unknown command '%s' use help to see list of available commands", arguments[0]);
				Log(message, LogType.Error);
			}

			strcpy(consoleInput, "");
			forceFocus = true;

			// Deallocate memory for words
			freeMemory(arguments, numWords);
			delete arguments;
		}

		static void Render(void* arguments) {
			if (!consoleOpen) return;
			ImGui::SetNextWindowSize(ImVec2(500, 100), ImGuiCond_FirstUseEver);
			ImGui::Begin("Console", &consoleOpen);

			const float footerHeightToReserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
			if (ImGui::BeginChild("ScrollRegion##", ImVec2(0, -footerHeightToReserve), false, 0))
			{
				ImGui::PushTextWrapPos();

				for (const auto &log : ConsoleLogs()) {
					ImGui::PushStyleColor(ImGuiCol_Text, outputColors[log.type]);
					ImGui::Text(log.message.c_str());
					ImGui::PopStyleColor();
				}

				if ((scrollToBottom && (ImGui::GetScrollY() >= ImGui::GetScrollMaxY()))) {
					ImGui::SetScrollHereY(1.0f);
					scrollToBottom = false;
				}

				ImGui::PopTextWrapPos();
				ImGui::EndChild();
			}

			ImGui::PushItemWidth(ImGui::GetWindowSize().x - ImGui::GetStyle().ItemSpacing.x * 2);
			if (forceFocus) {
				ImGui::SetKeyboardFocusHere(0);
				forceFocus = false;
			}

			if (ImGui::InputText("", consoleInput, sizeof(consoleInput), ImGuiInputTextFlags_EnterReturnsTrue)) {
				CommandExecuted(consoleInput);
			}
			ImGui::PopItemWidth();

			ImGui::End();
		}
	public:
		static void Update() {
			if (!zRender::Initialized()) return;
			if ((zKeyboard::WasKeyPressed(VK_OEM_3) || zKeyboard::WasKeyPressed(VK_F8))) {
				consoleOpen = !consoleOpen;
				forceFocus = true;
			}
		}

		static void Log(std::string message, int type) {
			LogItem newLog;
			newLog.type = type;
			newLog.message = message;
			ConsoleLogs().push_back(newLog);
			scrollToBottom = true;

			printf("[zConsole] %s\n", message.c_str());
		}

		static void AddCommandHandler(char* command, CommandCallback callback) {
			CommandHandler commandHandler;
			commandHandler.command = command;
			commandHandler.callback = callback;
			CommandHandlers().push_back(commandHandler);
		}

		static void Init() {
			zEvents::AddEventHandler("onImGuiRender", Render);

			outputColors[LogType.Default] = ImVec4(1, 1, 1, 1);
			outputColors[LogType.Error] = ImVec4(1, 0, 0, 1);
			outputColors[LogType.Warning] = ImVec4(1, 1, 0, 1);
			outputColors[LogType.Input] = ImVec4(.5, .5, .5, 1);

			char message[256];
			sprintf(message, "Gothic : GLua %s build %s %s", CURRENT_VERSION, __DATE__, __TIME__);
			Log(message, LogType.Default);
		}
	};
}