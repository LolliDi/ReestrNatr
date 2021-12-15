#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <stdio.h>

/*  */


WORD GetColor(LPWSTR c)
{
    switch (c[0])
    {
    case 'g': return FOREGROUND_GREEN;
    case 'b': return FOREGROUND_GREEN;
    case 'r': return FOREGROUND_RED;
    case 'W': return BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;
    case 'G': return BACKGROUND_GREEN;
    case 'B': return BACKGROUND_GREEN;
    case 'R': return BACKGROUND_RED;
    default:
        return FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;
    }
}

int SetFamily(LPWSTR c)
{
    switch (c[0])
    {
    case L'0':
        return 10;
        break;
    case L'1':
        return 11;
        break;
    case L'2':
        return 12;
        break;
    case L'3':
        return 13;
        break;
    case L'4':
        return 14;
        break;
    case L'5':
        return 15;
        break;
    case L'6':
        return 16;
        break;
    case L'7':
        return 17;
        break;
    case L'8':
        return 18;
        break;
   default:
        return 19;
        break;
    }

}

int main()
{
    system("chcp 1251>nul");
    CONSOLE_FONT_INFOEX cfi;
    memset(&cfi, 0, sizeof(cfi)); // выделяем память под структуру CONSOLE_FONT_INFOEX, в которой хранится информация о шрифтах
    cfi.cbSize = sizeof(cfi);
    GetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi); // записываем в структуру настройки днфолтных шрифтов шрифтов


    HKEY KeyConsoleSettings; //создаём дискрипторы для работы с реестром

    RegCreateKeyW(HKEY_CURRENT_USER, L"ConsoleSettings", &KeyConsoleSettings); //создаём в реестре папку для хранения настроек (и присваесае дискриптору его значение)

    // //заполняем папку параметрами
    //RegSetValueExW(KeyConsoleSettings, L"FontColor", NULL, REG_SZ, L"White", 6 * 2);
    //RegSetValueExW(KeyConsoleSettings, L"BackgroundColor", NULL, REG_SZ, L"Black", 6 * 2);
    //RegSetValueExW(KeyConsoleSettings, L"FontSize", NULL, REG_SZ, L"Classic", 8 * 2);


    LPWSTR StrSetting = calloc(20, 2);
    DWORD dataType = 0, dataLen = 40;
    char c = L'w', b = L'G';
    if (RegGetValueW(KeyConsoleSettings, NULL, L"FontFamily", RRF_RT_ANY, &dataType, StrSetting, &dataLen) == ERROR_SUCCESS)
    {
        LPWSTR font = calloc(20, sizeof(char));
        switch (StrSetting[0])
        {
        case 'g':
            font = L"MS Gothic";
            break;
        default:
            font = L"Consolas";
            break;
        }
        wcscpy_s(cfi.FaceName, 20 * 2, font);
        SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
    }
    else
    {
        RegSetValueExW(KeyConsoleSettings, L"FontFamily", NULL, REG_SZ, L"g", 8 * 2);
    }
    free(StrSetting);
    LPWSTR setFont = calloc(20, 2);
    LPWSTR setBack = calloc(20, 2);
    if (RegGetValueW(KeyConsoleSettings, NULL, L"FontColor", RRF_RT_ANY, &dataType, setFont, &dataLen) == ERROR_SUCCESS && RegGetValueW(KeyConsoleSettings, NULL, L"BackgroundColor", RRF_RT_ANY, &dataType, setBack, &dataLen) == ERROR_SUCCESS)
    {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), GetColor(setFont) | GetColor(setBack));
    }
    else
    {
        RegSetValueExW(KeyConsoleSettings, L"FontColor", NULL, REG_SZ, L"w", 2);
        RegSetValueExW(KeyConsoleSettings, L"BackgroundColor", NULL, REG_SZ, L"B", 2);
    }
    LPWSTR setSize = calloc(20, 2);
    if (RegGetValueW(KeyConsoleSettings, NULL, L"FontSize", RRF_RT_ANY, &dataType, setSize, &dataLen) == ERROR_SUCCESS)
    {
        cfi.dwFontSize.Y = SetFamily(setSize);
        SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
        
    }
    else 
    {
        RegSetValueExW(KeyConsoleSettings, L"FontSize", NULL, REG_SZ, L"4", 2);
    }
    system("cls");
    char* OUTPUTSTROKE = "Если указать несуществующий параметр - установится по умолчанию\nШрифт (нажать q на англ раскладке):\n- g - готик\n\nЦвета шрифта (нажать w на англ раскладке):\n- w = Белый\n- g = зеленый\n- b = синий\n- r = красный\n\nЦвета фона(нажать e на англ раскладке)\n- всё то же самое, но большими буквами\n\nРазмер шрифта(нажать r на англ раскладке)\n- от 0 до 9 (будет от 10 до 19)";
    printf("%s", OUTPUTSTROKE);
    wchar_t value[20];
    while (1) //цикл
    {
        StrSetting = calloc(20, 2);
        setFont = calloc(20, 2);
        setBack = calloc(20, 2);
        dataType = 0; dataLen = 40;
        switch (_getch())
        {
        case L'q':// тип щрифта Q
        case L'Q':
            printf("\nВведите тип шрифта: ");
           
            gets(value);
            LPWSTR val = calloc(20,sizeof(WCHAR));
            wsprintf(val, L"%c", value[0]);
            RegSetValueExW(KeyConsoleSettings, L"FontFamily", NULL, REG_SZ, val, 2);
            if (RegGetValueW(KeyConsoleSettings, NULL, L"FontFamily", RRF_RT_ANY, &dataType, StrSetting, &dataLen) == ERROR_SUCCESS)
            {
                LPWSTR font = calloc(20, sizeof(char));
                switch (StrSetting[0])
                {
                case 'g':
                    font = L"MS Gothic";
                    break;
                default:
                    font = L"Consolas";
                    break;
                }
                wcscpy_s(cfi.FaceName, 20 * 2, font);
                SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
            }
            else
            {
                MessageBox(NULL, L"Ошибка", L"Невозможно считать данные", MB_OK | MB_ICONERROR);
            }
            break;

            // цвет шрифта
        case L'w':
        case L'W':
            printf("\nВведите цвет шрифта: ");
            gets(value);
            wsprintf(StrSetting, L"%c", value[0]);
            RegSetValueExW(KeyConsoleSettings, L"FontColor", NULL, REG_SZ, StrSetting, 2);
            if (RegGetValueW(KeyConsoleSettings, NULL, L"FontColor", RRF_RT_ANY, &dataType, setFont, &dataLen) == ERROR_SUCCESS && RegGetValueW(KeyConsoleSettings, NULL, L"BackgroundColor", RRF_RT_ANY, &dataType, setBack, &dataLen) == ERROR_SUCCESS)
            {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), GetColor(setFont) | GetColor(setBack));
            }
            else
            {
                MessageBox(NULL, L"Ошибка", L"Невозможно считать данные", MB_OK | MB_ICONERROR);
            }
            break;

            // цвет заднего фона
        case L'E':
        case L'e':
            printf("\nВведите цвет фона: ");
            gets(value);
            wsprintf(StrSetting, L"%c", value[0]);
            RegSetValueExW(KeyConsoleSettings, L"BackgroundColor", NULL, REG_SZ, StrSetting, 2);
            if (RegGetValueW(KeyConsoleSettings, NULL, L"FontColor", RRF_RT_ANY, &dataType, setFont, &dataLen) == ERROR_SUCCESS && RegGetValueW(KeyConsoleSettings, NULL, L"BackgroundColor", RRF_RT_ANY, &dataType, setBack, &dataLen) == ERROR_SUCCESS)
            {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), GetColor(setFont) | GetColor(setBack));
            }
            else
            {
                MessageBox(NULL, L"Ошибка", L"Невозможно считать данные", MB_OK | MB_ICONERROR);
            }
            break;

        case L'r':// размер шрифта
        case L'R':
        case L'К':
        case L'к':
            printf("\nВведите размер шрифта: ");
            gets(value);
            wsprintf(StrSetting, L"%c", value[0]);
            RegSetValueExW(KeyConsoleSettings, L"FontSize", NULL, REG_SZ, StrSetting, 2);
            if (RegGetValueW(KeyConsoleSettings, NULL, L"FontSize", RRF_RT_ANY, &dataType, StrSetting, &dataLen) == ERROR_SUCCESS)
            {
                cfi.dwFontSize.Y = SetFamily(StrSetting);
                SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
            }
            else
            {
                MessageBox(NULL, L"Ошибка", L"Не возможно считать данные", MB_OK | MB_ICONERROR);
            }
            break;

        case L't':// выход
        case L'T':
        case L'Е':
        case L'е':
            RegCloseKey(KeyConsoleSettings);
            return 0;
        }
        free(StrSetting);
        free(setFont);
        free(setBack);
        system("cls");
        printf("%s", OUTPUTSTROKE);
    }
    RegCloseKey(KeyConsoleSettings);
    return 0;

}
