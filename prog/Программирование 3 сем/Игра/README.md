# ExOOP
Моя "любимая" реализация всякой срани для Чечикова

Для вас, дорогой читатель, который знает только то, что показывал Чечиков, следует обьяснение:

## Как это работает?
Структура проекта достаточно сложная на первый взгляд. Много папок и всяких приколов, обьясню как оно собирается и что вообще делает

Существует такая штука, как **cmake** это удобный способ собирать многофайловые проекты, потому что по написанному единожды файлу конфигурации мы получаем готовый исполняемый файл по одному клику и радуемся.

У него есть **CMakeLists.txt** он конфигурируется особым образом и делает красиво. Естесственно никто не знает его синтаксис, так что делаем через нейронку и радуемся.

CMake собирает проект из всех **.cpp**-файлов, перечисленных в **CMakeLists.txt**, и делает exe файлик.

## Структура проекта
В базовой директории есть main.cpp и парочка папок, рассмотрим их:
```
├── .zed
│   ├── tasks.json        # задачи Zed: F5 / Alt+F6
│   └── build-run.ps1     # configure + build + run через CMake
├── build                 # старая сборка с macOS (не используется на Windows)
├── build-win             # сборка на Windows (создаётся задачей, в .gitignore)
├── cmake
│   ├── CMakeLists.txt
│   └── toolchain.cmake   # теперь работает и на macOS, и на Windows
├── gameSettings.h
├── main.cpp
├── mBaseHierarhy
│   ├── TLocation.cpp
│   ├── TLocation.h
│   ├── TPoint.cpp
│   └── TPoint.h
├── mObjects
│   ├── TRepairZone.cpp
│   ├── TRepairZone.h
│   ├── TRockets.cpp
│   ├── TRockets.h
│   ├── TRocketsFill.cpp
│   ├── TRocketsFill.h
│   └── TStarBonus.h
├── mPlaneHierarhy
│   ├── TBrokenPlane.cpp
│   ├── TBrokenPlane.h
│   ├── TMagicPlane.cpp
│   ├── TMagicPlane.h
│   ├── TPlane.cpp
│   ├── TPlane.h
│   ├── TRepairedPlane.cpp
│   └── TRepairedPlane.h
├── mTowerHierarhy
│   ├── TMagicTower.cpp
│   ├── TMagicTower.h
│   ├── TTower.cpp
│   └── TTower.h
├── output
│   └── ExOOP.exe
├── .gitignore
└── README.md
```
* main.cpp - наш классический main где мы пишем саму программу
* gameSettings.h - настройки игры (размер окна, FPS, число башен и тд.)
* cmake - папка с конфигурационными файлами cmake
* mBaseHierarhy - базовая иерархия, содержит точку и локацию
* mPlaneHierarhy, mTowerHierarhy, mObjects - иерархии обьектов, они строятся над базовой и все содержат коренным классом *интерфейс*
* .zed - настройки задач Zed: F5 собирает и запускает, Alt+F6 только собирает

Ну вот собственно и всё.

## Как собирать проект?

### Windows + Zed (сейчас основной вариант)

1. Откройте папку **Игра** в Zed именно как корень проекта — иначе задачи из `.zed/tasks.json` не найдутся.
2. Нажмите **F5**: задача `Compile & Run (C)` сделает configure, build и запустит игру.
3. **Alt+F6** — только собрать (`Compile only (C)`), без запуска.
4. Задача `Configure (CMake)` — только переконфигурировать проект (обычно не нужна).

Нужны **CMake** (3.20+), **Ninja** и **MinGW-w64** с `g++`. CMake стоит в `C:\Program Files\CMake\bin`, Ninja — в `%LOCALAPPDATA%\Programs\ninja`, MinGW — в `C:\Users\user\gcc\bin`. Скрипт `.zed/build-run.ps1` сам добавляет эти папки в PATH, поэтому перезапускать Zed после установки не обязательно.

Почему Ninja, а не обычные Makefiles: генератор `Unix Makefiles` на Windows спотыкается о кириллический путь к проекту (`Target DependInfo.cmake file not found`), а Ninja с этим работает как надо.

Сборка идёт в папку **build-win/** (старая **build/** осталась от macOS-сборки и на Windows не используется). Готовый exe — `output/ExOOP.exe`, выйти из игры — **ESC**.

### Про окно игры
Игра открывает отдельное окно **ExOOP - Plane Game** с обычной рамкой: его можно двигать, сворачивать и закрывать кнопкой ×. Запуск из cmd, Windows Terminal и встроенного терминала работает одинаково.

Раньше `main.cpp` брал `HDC` у `GetConsoleWindow()` и рисовал прямо в консоль. В классическом `cmd.exe` это работало, но в Windows Terminal / терминале Zed консоль рисуется через ConPTY, и GDI-рисование в неё не отображается — экран оставался чёрным. Теперь игра сама регистрирует класс окна (`RegisterClassExA`), создаёт окно (`CreateWindowExA`) и рисует в его `HDC`; сообщения окна обрабатываются прямо в игровом цикле. Позиция и размер окна задаются в `gameSettings.h` (`windowX`, `windowY`, `windowW`, `windowH`).

### macOS + VSCode (как было изначально)

Ставите расширение CMakeTools, указываете ему `cmake/CMakeLists.txt` и жмёте снизу build или run. У вас скомпилится или запустится ваша любимая программа. Файл `cmake/toolchain.cmake` теперь сам определяет систему: на macOS он по-прежнему кросс-компилирует в Windows через Homebrew MinGW-w64, а на Windows берёт локальный MinGW.

> Если вдруг захотите CMakeTools на Windows: поправьте `.vscode/settings.json`, там пока зашиты macOS-пути (`/opt/homebrew/bin/cmake` и старая папка исходников). Для Zed это не нужно.

# Продумки про стилистику игры
Так, ну делаем простенькую игру, где самолет умеет срелять и летит и маневрирует мимо башен. Маневры будут в строго отведенной зоне, башни будут разные и давать разные свойства, также добавим отдельную иерархию для бонусных обьектов. Жизней будет несколько, при столкновении обьект начинает мигать и меняется.

## TODO

1) Сделать перемещение самолета
2) Сделать перемещение башен и генерацию их в различных местах
3) 

## Заметки для будущих агентов (подводные камни)

Этот раздел не для читателя, а для того, кто будет снова править проект. Здесь собрано всё, что выяснилось в процессе настройки, чтобы не наступать на те же грабли.

### Окружение
* ОС Windows, путь к проекту содержит кириллицу: `Программирование 3 сем\Игра`.
* CMake 4.5.1 стоит в `C:\Program Files\CMake\bin\cmake.exe`, но при установке PATH не обновился — поэтому `.zed/build-run.ps1` добавляет пути к CMake, Ninja и MinGW в PATH сам.
* Ninja 1.12.1 скачан с GitHub и лежит в `%LOCALAPPDATA%\Programs\ninja\ninja.exe`.
* MinGW-w64: `g++` 15.1.0 в `C:\Users\user\gcc\bin`.
* Глобальный keymap Zed — `%APPDATA%\Zed\keymap.json`, контекст `Workspace`: `f5` → `task::Spawn` с `task_name = "Compile & Run (C)"`, `alt-f6` → `"Compile only (C)"`. **Не переименовывайте label задач в `.zed/tasks.json`**, иначе эти биндинги перестанут работать.

### Сборка
* Zed должен открывать папку **Игра** как корень проекта: задачи берутся из `<root>/.zed/tasks.json`.
* `.zed/build-run.ps1` делает configure (`-S cmake -B build-win -G Ninja -DCMAKE_TOOLCHAIN_FILE=cmake/toolchain.cmake`), build и run.
* `cmake/toolchain.cmake` теперь сам определяет систему: на macOS сохранена изначальная кросс-компиляция через Homebrew MinGW-w64, на Windows (`CMAKE_HOST_WIN32`) ищет локальные `g++`/`gcc`. Пути `/opt/homebrew/...` на Windows использовать нельзя.
* **Не используйте генератор `Unix Makefiles`**: на кириллическом пути CMake падает с `Target DependInfo.cmake file not found`. `chcp 65001` не помогает (проверено). ASCII-симлинк через `mklink /J` помогает, но мы оставили Ninja как более чистое решение.
* `CMakeLists.txt` линкует статически (`-static -static-libgcc -static-libstdc++`). `gdi32` CMake подключает автоматически через стандартные библиотеки MinGW; при сборке напрямую через `g++` нужен явный `-lgdi32`, иначе `undefined reference to __imp_CreatePen/BitBlt/...`.
* Папка `build/` — старый кэш, созданный на macOS (`CMAKE_GENERATOR=Ninja`, компилятор `/opt/homebrew/bin/...`); не трогать и не переиспользовать. Рабочая папка — `build-win/`, она добавлена в `.gitignore`.

### Окно игры
* Активный `main()` в конце `main.cpp` создаёт своё окно Win32 (`RegisterClassExA` → `CreateWindowExA`, стиль `WS_OVERLAPPEDWINDOW`) и рисует через `GetDC(hwnd)`.
* В начале файла остался первый вариант `main()` целиком в комментарии — это история, его не раскомментировать: он использует `GetConsoleWindow()`.
* Причина переделки: GDI-рисование в консоль работает только в классическом `cmd.exe` (conhost). В Windows Terminal / терминале Zed консоль работает через ConPTY, и `GetWindowDC(GetConsoleWindow())` рисует в невидимое окно — экран остаётся чёрным. Пользователь выбрал именно отдельное окно Win32 (не ASCII-рендер и не Qt).
* В игровом цикле обязательна обработка сообщений (`PeekMessage` / `TranslateMessage` / `DispatchMessage`), иначе окно не двигается и не закрывается. `WM_CLOSE` выставляет `gameRunning = false`, ESC тоже выходит из игры.
* Размер клиентской области подгоняется через `AdjustWindowRectEx`, чтобы совпадать с `windowW` / `windowH` из `gameSettings.h`.

### ВАЖНО: output/ExOOP.exe отслеживается git ⚠️
* `output/ExOOP.exe` лежит в git, и это главный источник путаницы.
* **Никогда не делайте `git restore output/ExOOP.exe` «для чистоты»**: в коммите хранится старый бинарник до исправления окна; после отката игра снова ничего не показывает. Мы именно на этом и попались.
* Ninja считает откатанный файл свежим и пишет `no work to do`, поэтому сам он не пересоберётся. Чтобы пересобрать принудительно: удалите `output/ExOOP.exe` и запустите задачу заново — в логе должно быть `[1/1] Linking CXX executable`.
* Запущенные экземпляры игры блокируют exe: перед удалением/пересборкой завершите процессы `ExOOP` (`Stop-Process -Name ExOOP`). Зависших процессов может быть несколько (было 5 штук).
* Признак рабочего бинарника: размер ~13,6 МБ и `MainWindowTitle = "ExOOP - Plane Game"`. У старого ~12,9 МБ и окна нет.

### Как проверить, что всё работает
1. F5 → в логе `==> Build OK`, затем `==> Running ExOOP.exe`.
2. `Get-Process ExOOP | Select MainWindowTitle` → `ExOOP - Plane Game`.
3. Окно перемещается через `SetWindowPos` и закрывается через `WM_CLOSE` с кодом выхода 0.
4. Скриншот области окна показывает HUD `Level 1 ...`, зелёную полоску здоровья и зоны. `PrintWindow` может вернуть чёрное изображение — используйте `CopyFromScreen`.

### Прочее
* `.vscode/settings.json` до сих пор содержит macOS-пути (`/opt/homebrew/bin/cmake` и старый `sourceDirectory`); в Zed он не используется. Если понадобится VSCode на Windows — поправить отдельно.