/*		AutoLogin for WoW
*		by Daniel Maciag
*		
*		This program uses Windows API to automate the login process for users
*		by simulating virtual key presses.
*/

#include <iostream>
#include <string>
#include <windows.h>
#include <tchar.h>
#include <conio.h>
#include <stdio.h>
#include <string.h>

using namespace std;

DWORD dwProcessId;
HANDLE hWoW;
HWND hWnd;

string user = "username@gmail.com";
string pass = "hokag00";
string userBegin;
string userEnd;

HANDLE grabWoW() {
	hWnd = FindWindow(NULL, "World of Warcraft");
	GetWindowThreadProcessId(hWnd, &dwProcessId);
	hWoW = OpenProcess(PROCESS_ALL_ACCESS, 0, dwProcessId);
	return hWoW;
}

//split up username before and after the @ symbol into two parts.
void BreakUpUserInfo(string user) {
	int userIndex = 0;
	//beginning username
	for (int u = 0; u < user.length(); u++) {
		if (user[u] == '@') {
			userIndex = ++u;
			break;
		}
		else {
			userBegin += user[u];
		}
	}
	//ending username
	while(user[userIndex]){
		userEnd += user[userIndex];
		userIndex++;
	}
}

void EnterLoginInfo(string userBegin, string userEnd, string pass) {
	INPUT in;
	in.type = INPUT_KEYBOARD;
	//Username Beginning
	for (int u = 0; u < userBegin.length(); u++) {
		in.ki.wVk = VkKeyScan(userBegin[u]);
		in.ki.dwFlags = 0;
		SendInput(1, &in, sizeof(INPUT));
		in.ki.dwFlags = KEYEVENTF_KEYUP;
		SendInput(1, &in, sizeof(INPUT));
	}
	//@ symbol in username
	in.ki.wVk = VK_SHIFT;
	in.ki.dwFlags = 0;
	SendInput(1, &in, sizeof(INPUT));
	in.ki.wVk = 0x32;
	in.ki.dwFlags = 0;
	SendInput(1, &in, sizeof(INPUT));
	in.ki.wVk = 0x32;
	in.ki.dwFlags = KEYEVENTF_KEYUP;
	in.ki.wVk = VK_SHIFT;
	in.ki.dwFlags = KEYEVENTF_KEYUP;
	SendInput(1, &in, sizeof(INPUT));
	//Username Ending
	for (int u = 0; u < userEnd.length(); u++) {
		in.ki.wVk = VkKeyScan(userEnd[u]);
		in.ki.dwFlags = 0;
		SendInput(1, &in, sizeof(INPUT));
		in.ki.dwFlags = KEYEVENTF_KEYUP;
		SendInput(1, &in, sizeof(INPUT));
	}
	//TAB
	in.ki.wVk = VK_TAB;
	in.ki.dwFlags = 0;
	SendInput(1, &in, sizeof(INPUT));
	in.ki.dwFlags = KEYEVENTF_KEYUP;
	SendInput(1, &in, sizeof(INPUT));
	//Password
	for (int p = 0; p < pass.length(); p++) {
		if (pass[p] >= 'A' && pass[p] <= 'Z') {
			//for capital letters 
			in.ki.wVk = VK_SHIFT;
			in.ki.dwFlags = 0;
			SendInput(1, &in, sizeof(INPUT));
		}
		in.ki.wVk = VkKeyScan(pass[p]);
		in.ki.dwFlags = 0;
		SendInput(1, &in, sizeof(INPUT));
		in.ki.dwFlags = KEYEVENTF_KEYUP;
		SendInput(1, &in, sizeof(INPUT));
		if (pass[p] >= 'A' && pass[p] <= 'Z') {
			//for capital letters 
			in.ki.dwFlags = KEYEVENTF_KEYUP;
			SendInput(1, &in, sizeof(INPUT));
		}
	}
	//Enter
	in.ki.wVk = VK_RETURN;
	in.ki.dwFlags = 0;
	SendInput(1, &in, sizeof(INPUT));
	in.ki.dwFlags = KEYEVENTF_KEYUP;
	SendInput(1, &in, sizeof(INPUT));
}

void HelpScreen() {
	cout << "Welcome to the Help Screen!" <<endl;
	cout << "Press C to Login to the Game" << endl;
}
void WaitForInput() {//fix
	while (true) {
		if (GetAsyncKeyState(0x45)) {
			Sleep(1000);
			cout << "Entering user/pass...." << endl;
			SwitchToThisWindow(hWnd, TRUE);
			if (hWnd)EnterLoginInfo(userBegin, userEnd, pass);
			if (!GetAsyncKeyState(0x45)) {
				break;
			}
		}
		if (GetAsyncKeyState(VK_ESCAPE)) {
			break;
		}
	}
}
int main() {
	//names window
	HWND terminalhWND = GetConsoleWindow();
	SetWindowText(terminalhWND, (LPCTSTR) "AutoLogin 1.0.0");

	//color terminal to green
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
	cout << "Welcome to AutoLogin 1.0.0" << endl;
	Sleep(3000);
	system("cls");
	grabWoW();
	while (!grabWoW()) {
		cout << "Please start Wow.";
		Sleep(1000);
		
		if (grabWoW()) {
			cout << "Found Wow.\n";
			grabWoW();
			break;
		}
	}
	SwitchToThisWindow(hWnd, TRUE);
	BreakUpUserInfo(user);
	WaitForInput();
	
	cin.ignore();
	cin.ignore();
	return 0;
}