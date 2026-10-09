# Build Tool EN

Program has next functions:
1. If needed, compiles sources and install them in target directory.
2. If needed, recompiles assets and import them in asset database.
3. Archiving assets into single package.

## Arguments

### Basic arguments 
| Key       | Description                                                                                   |
| --------- | --------------------------------------------------------------------------------------------- |
|`--log`    | Log output path or current exe directory                                                      |
|`--verbose`| Verbose output                                                                                |
|`--help`   | Display this help screen.                                                                     |

### Build arguments (***build***)
| Key                                | Description                                                                                       |
| ---------------------------------- | ------------------------------------------------------------------------------------------------- |
| `--project`                        |   **Required.** Target Project Path                                                               |
| `--config`                         |   Debug or Release (or other if exits) build config (Debug by default)                            |
| `--targetDir`                      |   **Required.** Target directory                                                                  |
| `--openAfterBuild`                 |   Opens directory after build done                                                                |

## Exit codes for cmd users 

| Code | Name                               |  Description                                                      |
| ---  | ---------------------------------- | ----------------------------------------------------------------- |
|  0   | Success                            | Project generated successfully                                    |
|  1   | InternalError                      | Reserved for programm internal usage only                         |
|  2   | CMakeGenerationFailed              | Failed to generate CMake project (but game project were created!) |
|  3   | InvalidArgumentsError              | Provided not suitable arguments                                   |
|  4   | InvalidPathError                   | Project path in invalid                                           |
|  5   | ProjectGenerationFailed            | Failed to generate a project (see log for details)                |
|  6   | ProjectBuildFailed                 | Failed to build a project (see log for details)                   |
|  7   | SaveProjectFailed                  | Failed to save a project (see log for details)                    |
|  8   | FailedToOverrideJsonProperties     | Failed to override settings properties (see log for details)      |

# Build Tool RU

Программа имеет следующие функции:
1. Если необходимо, компилирует исходные файлы и устанавливает в целевую директорию
2. Импортирует и компилирует ассеты в базу данных ассетов если необходимо.
3. Архивирует ассеты в архив

## Аргументы

### Basic arguments 
| Key       | Description                                                                                                  |
| --------- | ------------------------------------------------------------------------------------------------------------ |
|`--log`    | Путь к папке в которую сохранять логи (по умолчанию текущий пусть программы)                                 |
|`--verbose`| Выводить ли полный лог                                                                                       |
|`--help`   | Показать все аргументы.                                                                                      |

### Build arguments (***build***)
| Key                                | Description                                                                                                 |
| ---------------------------------- | ----------------------------------------------------------------------------------------------------------- |
| `--project`                        |   **Обязателен.** Путь к проекту                                                                            |
| `--config`                         |   Кофигурация сборки (Default по умолчанию)                                                                 |
| `--targetDir`                      |   **Required.** Target directory                                                                            |
| `--openAfterBuild`                 |   Окрытие директории в проводнике после сборки                                                              | 

## Exit-коды для внешних вызовов

| Code | Name                               |  Description                                                      |
| ---  | ---------------------------------- | ----------------------------------------------------------------- |
|  0   | Success                            | Успешная генерация проекта                                        |
|  1   | InternalError                      | Зарезервированно для внутреннего использования                    |
|  2   | CMakeGenerationFailed              | Ошибка генерации CMake (но игровой проект успешно сгенерирован!)  |
|  3   | InvalidArgumentsError              | Переданы неверные аргументы                                       |
|  4   | InvalidPathError                   | Путь к проекту неверен                                            |
|  5   | ProjectGenerationFailed            | Неуспешная генерация проекта (см. лог для деталей)                |
|  6   | ProjectBuildFailed                 | Неуспешная сборка проекта (см. лог для деталей)                   |
|  7   | SaveProjectFailed                  | Неуспешная попытка сохранения проекта (см. лог для деталей)       |
|  8   | FailedToOverrideJsonProperties     | Неуспешная попытка слияния конфига проекта (см. лог для деталей)  |


# Example / Примеры

```
--verbose build --project C:\Projects\Test\MyProject --config Release  --openAfterBuild --targetDir C:\Projects\Test\Out
```

