//нажатие клавиш без поля ввода
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QTime>
#include <QChar>
#include <Windows.h>
#include <iostream>
#pragma comment(lib, "user32.lib")

HHOOK hHook = NULL;
using namespace std;

void UpdateKeyState(BYTE *keystate, int keycode)
{
    keystate[keycode] = GetKeyState(keycode);
}

LRESULT CALLBACK MyLowLovelKeyBoardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    qDebug() << "кнопка нажата";

    KBDLLHOOKSTRUCT cKey = *((KBDLLHOOKSTRUCT*)lParam);
    wchar_t buffer[5];
    BYTE keyboard_state[256];
    GetKeyboardState(keyboard_state);
    UpdateKeyState(keyboard_state, VK_SHIFT);
    UpdateKeyState(keyboard_state, VK_CAPITAL);
    UpdateKeyState(keyboard_state, VK_CONTROL);
    UpdateKeyState(keyboard_state, VK_MENU);
    HKL keyboard_layout = GetKeyboardLayout(0);
    char lpszName[0x100] = {0};
    DWORD dwMsg = 1;
    dwMsg += cKey.scanCode << 16;
    dwMsg += cKey.flags << 24;
    int i = GetKeyNameText(dwMsg, (LPTSTR)lpszName, 255);
    int result = ToUnicodeEx(cKey.vkCode, cKey.scanCode, keyboard_state, buffer, 4, 0, keyboard_layout);
    buffer[4] = L'\0';
    qDebug() << "key " << cKey.vkCode << " " << QString::fromUtf16((ushort*)buffer) << " " << QString::fromUtf16((ushort*)lpszName);
    return CallNextHookEx(hHook, nCode, wParam, lParam);
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    hHook = SetWindowsHookEx(WH_KEYBOARD_LL, MyLowLovelKeyBoardProc, NULL, 0);
    if(hHook == NULL) {
        qDebug() << "hook failed";
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}
