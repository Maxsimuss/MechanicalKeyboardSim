#include <Windows.h>
#include <format>
#include <string>
#include <iostream>
#include <filesystem>
#include <sstream>
#include <vector>
#include <set>
#include "AudioEngine.h"

#define TRAY_WINAPI 1
#include "tray.h"

AudioEngine* audioEngine;
std::set<DWORD> pressedKeys;
bool running = true;

void onKeyEvent(DWORD keyCode, bool isKeyDown) {
	std::string state = isKeyDown ? "down" : "up";
	if (isKeyDown) {
		if (pressedKeys.contains(keyCode)) {
			return;
		}
		else {
			pressedKeys.insert(keyCode);
		}
	}
	else {
		pressedKeys.erase(keyCode);
	}

	switch (keyCode) {
		case VK_SPACE:
			audioEngine->playSound("space-" + state);
			break;
		case VK_RETURN:
			audioEngine->playSound("enter-" + state);
			break;
		case VK_BACK:
			audioEngine->playSound("backspace-" + state);
			break;
		case VK_LSHIFT:
		case VK_RSHIFT:
		case VK_CAPITAL:
		case VK_TAB:
			audioEngine->playSound("shift-" + state);
			break;
		default:
			audioEngine->playSound("normal-" + state);
			break;
	}
}

LRESULT CALLBACK keyboardHook(int nCode, WPARAM wParam, LPARAM lParam) {
	if (nCode == HC_ACTION) {
		if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) {
			onKeyEvent(((PKBDLLHOOKSTRUCT)lParam)->vkCode, true);
		}
		else if (wParam == WM_KEYUP || wParam == WM_SYSKEYUP) {
			onKeyEvent(((PKBDLLHOOKSTRUCT)lParam)->vkCode, false);
		}
	}
	return CallNextHookEx(NULL, nCode, wParam, lParam);
}

void exit(tray_menu* item) {
	running = false;
}

void setupTray() {
	tray tray;
	std::filesystem::path iconPath = std::filesystem::current_path() / "resources/icon.ico";
	std::string icoPathString = iconPath.string();

	tray.icon = new char[icoPathString.length() + 1];
	strcpy_s(tray.icon, icoPathString.length() + 1, icoPathString.c_str());

	tray_menu data[] = {
		{(char*)"Exit", 0, 0, exit, NULL},
		{NULL, 0, 0, NULL, NULL},
	};
	tray.menu = data;

	tray_init(&tray);
}

void setupAudio() {
	audioEngine = new AudioEngine();

	std::filesystem::path path = std::filesystem::current_path() / "resources/keys/";
	for (const auto& entry : std::filesystem::directory_iterator(path)) {
		if (!entry.is_regular_file()) continue;

		std::string filename{entry.path().filename().string()};
		filename = filename.substr(filename.find_last_of("/\\") + 1);
		filename = filename.substr(0, filename.find_last_of('.'));

		std::vector<std::string> parts;
		std::istringstream f(filename);
		std::string s;
		while (getline(f, s, '-')) {
			parts.push_back(s);
		}

		std::string key = parts[0];
		std::string state = parts[1];

		audioEngine->registerSound(std::format("{}-{}", key.c_str(), state.c_str()), entry.path().string());
	}
}

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
	setupTray();
	setupAudio();

	HHOOK hook = SetWindowsHookExA(WH_KEYBOARD_LL, keyboardHook, 0, 0);

	while (running && tray_loop(1) == 0);

	UnhookWindowsHookEx(hook);
	tray_exit();
}