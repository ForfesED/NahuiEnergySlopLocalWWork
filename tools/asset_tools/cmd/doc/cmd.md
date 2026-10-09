# Asset Tool EN

Program has next functions:
1. Scans a project and verify metadata for each asset.
2. Compile assets into assets database.

## Arguments

### Basic arguments 
| Key       | Description                                                                                   |
| --------- | --------------------------------------------------------------------------------------------- |
|`--log`    | Log output path or current exe directory                                                      |
|`--verbose`| Verbose output                                                                                |
|`--help`   | Display this help screen.                                                                     |

### Asset importing arguments (***import***)
| Key                                | Description                                                                                       |
| ---------------------------------- | ------------------------------------------------------------------------------------------------- |
| `--project`                        |   **Required.** Target Project Path                                                               |
| `--file`                           |   Path to the specific file (if none, all assets will be processed)                               |
| `--files_mask`                     |   List of specific extensions to scan (none by default)                                           |

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

# Asset Tool RU

Программа имеет следующие функции:
1. Сканирует проект и проверяет метаданные для каждого ассета.
2. Импортирует и компилирует ассеты в базу данных ассетов.

## Аргументы

### Basic arguments 
| Key       | Description                                                                                                  |
| --------- | ------------------------------------------------------------------------------------------------------------ |
|`--log`    | Путь к папке в которую сохранять логи (по умолчанию текущий пусть программы)                                 |
|`--verbose`| Выводить ли полный лог                                                                                       |
|`--help`   | Показать все аргументы.                                                                                      |

### Asset import arguments (***import***)
| Key                                | Description                                                                                                 |
| ---------------------------------- | ----------------------------------------------------------------------------------------------------------- |
| `--project`                        |   **Обязателен.** Путь к проекту                                                                            |
| `--file`                           |   Путь к файлу  (если нет, все ассеты будут обработаны)                                                     |
| `--files_mask`                     |   Список расширений файлов для их исключительного включения в сканирование (по умолчанию пусто)             |

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
asset_tool --verbose import --project C:\Projects\Test
```

