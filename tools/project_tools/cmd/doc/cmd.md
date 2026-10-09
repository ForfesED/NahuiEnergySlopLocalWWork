# Project Generation Tool EN

Program has next functions:
1. Generates a game project from the template and optionally with console arguments provided generates CMake code project.
2. Re-generates CMake project
3. Upgrades project version
4. Writes and merge configs of nauproject file

## Arguments

### Basic arguments 
| Key       | Description                                                                                   |
| --------- | --------------------------------------------------------------------------------------------- |
|`--log`    | Log output path or current exe directory                                                      |
|`--verbose`| Verbose output                                                                                |
|`--tools`  | Path to tools directory (if not provided, tries to search in parent dirs, fails if non-found) |
|`--help`   | Display this help screen.                                                                     |

### Project creation arguments (***init***)
| Key             | Description                                                                             |
| --------------- | --------------------------------------------------------------------------------------- |
| `--project`     |   **Required.** Target Project Path                                                     |
| `--preset`      |   Preset for CMake build (default is win_vs2022_x64)                                    |
| `--name`        |   Project name (Uses default value if not provided)                                     |
| `--template`    |   **Required.** Template name                                                           |
| `--contentOnly` |   Content only type of project                                                          |
| `--generate`    |   Automatically generate solution at the end                                            |
| `--openIde`     |   Automatically opens ide at end                                                        |

### Regenerate solution arguments (***clean_rebuild***)
| Key             | Description                                                                             |
| --------------- | --------------------------------------------------------------------------------------- |
| `--project`     |   **Required.** Target Project Path                                                     |
| `--preset`      |   Preset for CMake build (default is win_vs2022_x64)                                    |
| `--name`        |   Project name (Uses default value if not provided)                                     |
| `--openIde`     |   Automatically opens ide at end                                                        |

### Upgrade project arguments (***upgrade***)
| Key                    | Description                                                                       |
| ---------------------- | --------------------------------------------------------------------------------- |
| `--project`            |   **Required.** Target Project Path                                               |
| `--name`               |   Project name (Uses default value if not provided)                               |
| `--v`                  |   Which version should this save use?                                             |
| `--do-not-upgrade`     |   If provided, asset builder will not be called                                   |

### Save project arguments (***save***)
| Key                    | Description                                                                       |
| ---------------------- | --------------------------------------------------------------------------------- |
| `--project`            |   **Required.** Target Project Path                                               |
| `--name`               |   Project name (Uses default value if not provided)                               |
| `--cfg`                |   Escaped JSON params string with new values?                                     |
| `--do-not-upgrade`     |   If provided, asset builder will not be called                                   |


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

# Project Generation Tool RU

Программа имеет следующие функции:
1. Генерирует проект игры на основе шаблона и, при необходимости, с предоставленными аргументами, генерирует проект CMake.
2. Повторно генерирует проект CMake.
3. Обновляет версию проекта c апргейдом ассетов до актуальной версии (даунгрейд не поддерживается).
4. Записывает и объединяет конфигурации файла nauproject.

## Аргументы

### Basic arguments 
| Key       | Description                                                                                                  |
| --------- | ------------------------------------------------------------------------------------------------------------ |
|`--log`    | Путь к папке в которую сохранять логи (по умолчанию текущий пусть программы)                                 |
|`--verbose`| Выводить ли полный лог                                                                                       |
|`--tools`  | Путь к папке с инструментами (Если нет, то будет предпринята попытка найти папку в родительских директориях) |
|`--help`   | Показать все аргументы.                                                                                      |

### Project creation arguments (***init***)
| Key             | Description                                                                             |
| --------------- | --------------------------------------------------------------------------------------- |
| `--project`     |   Путь к папке где создать игровой проект  (**обязательный аргумент**)                  |
| `--preset`      |   Название пресета для генерации CMake (по умолчанию win_vs2022_x64)                    |
| `--name`        |   Название игрового проекта                                                             |
| `--template`    |   Назание темплейта проекта (**обязательный аргумент**)                                 |
| `--contentOnly` |   Если true, сгенерирует проект без кода                                                |
| `--generate`    |   Генерирует проект CMake после генерации игрового проекта                              |
| `--openIde`     |   Открыть Ide после генерации проекта?                                                  |

### Regenerate solution arguments (***clean_rebuild***)
| Key             | Description                                                                             |
| --------------- | --------------------------------------------------------------------------------------- |
| `--project`     |   Путь к папке c игровым проектом  (**обязательный аргумент**)                          |
| `--preset`      |   Название пресета для генерации CMake (по умолчанию win_vs2022_x64)                    |
| `--name`        |   Название игрового проекта                                                             |
| `--openIde`     |   Открыть Ide после генерации проекта?                                                  |

### Upgrade project arguments (***upgrade***)
| Key                    | Description                                                                       |
| ---------------------- | --------------------------------------------------------------------------------- |
| `--project`            |   Путь к папке c игровым проектом  (**обязательный аргумент**)                    |
| `--name`               |   Название игрового проекта                                                       |
| `--ver`                |   Версия, к которой нужно привести проект                                         |
| `--do-not-upgrade`     |   Если предоставлен, сборка ассетов будет пропущенна                              |

### Save project arguments (***save***)
| Key                    | Description                                                                       |
| ---------------------- | --------------------------------------------------------------------------------- |
| `--project`            |   Путь к папке c игровым проектом  (**обязательный аргумент**)                    |
| `--name`               |   Название игрового проекта                                                       |
| `--cfg`                |   Экранированная строка JSON с параметрами к сохранению или перезаписи            |
| `--do-not-upgrade`     |   Если предоставлен, сборка ассетов будет пропущенна                              |

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
ProjectToolCmd.exe --verbose init --template empty --project C:\Projects\Test --name Test

```

