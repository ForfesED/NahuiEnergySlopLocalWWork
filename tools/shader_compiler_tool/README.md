# Shader Compiler Tool EN

## Description

The Shader Compiler Tool can:
1. Compile an HLSL file and save a binary file containing byte-code, reflection, and a header with shader information.
2. Build a shader cache in the form of separate binary files for each shader in a specified directory.
3. Build a single shader cache file containing all compiled shaders from a specified directory.
4. Compile a specified pair of shader and metafile, and save the compiled result in a single shader cache file.
5. Embed debug information into the shader bytecode for easier debugging.
6. Output PDB files containing debugging symbols to a specified directory for use with debugging tools.

## Arguments

| Key   | Full Key        | Description                                                                 |
|-------|-----------------|-----------------------------------------------------------------------------|
| `-o`  | `--out`         | **Required.** Path to the output directory for shader cache                 |
| `-s`  | `--shaders`     | **Required.** Path to the folder with shaders or path to single source      |
| `-m`  | `--metafiles`   | **Required.** Path to the folder with metafiles or path to single metafile  |
| `-i`  | `--includes`    | Optional. List of paths for searching headers (.h or .hlsli) for shaders    |
| `-c`  | `--cache`       | Optional. Name for shader cache file                                        |   
| `De`  | `--debug-embed` | Optional. Embed debug information into the shader bytecode                  |
| `Do`  | `--debug-out`   | Optional. Directory path to save PDB files for debugging                    |
| `h`   | `--help`        | Display help message and exit                                               |

### Usage

```sh
ShaderCompilerTool.exe -o <output_directory> -s <shaders_path> -m <metafiles_path> [-i <include_path1> <include_path2> ...] [-c <shader_cache_name>] [-Do <pdb_output_directory>] [-De]
```

## Requirements

Each HLSL file must have a corresponding .blk metafile that contains information about the stage(s) for which the shader 
will be compiled (_vertex_, _pixel_, etc.) and information about permutations and defines for each.
There must be at least one permutation named `Regular` (with or without defines).
If there are other permutations and `Regular` is not needed, it can be omitted. At least one shader stage must be specified.
All files can be in different directories or in the same one. The metafile must have the same name as the shader.

## How it works

If both the shader and metafile are specified as files, only that pair will be compiled. All compiled variants will be
stored in a single shader cache file, named according to the `-c` parameter or, if this parameter is not provided, the
shader's name will be used.

If both paths are directories, the tool collects the paths of all metafiles and HLSL files, then matches them by name.
It then compiles each HLSL file according to its metadata.
Compiled files are stored in a temporary folder (which will be deleted upon completion).
When all files are compiled without errors, the tool either creates separate binary files for each shader in the output
directory or a single shader cache file, depending on whether `-c` or `--cache` is specified.

If the `-De` or `--debug-embed` flag is used, the tool will embed debug information into the shader bytecode,
facilitating easier debugging of shaders. If the `-Do` or `--debug-out` flag is used, the tool will output PDB files 
containing debugging symbols to the specified directory. This directory will be used for debugging tools to provide 
additional context during development. The `-De` and `-Do` flags can be used together. When both flags are specified,
the tool will embed debug information into the shader bytecode and also generate separate PDB files for additional
debugging context.

If compilation or cache building fails, the process stops and an error message is displayed. If successful, a success
message and the full path to the shader cache file or directory will be displayed.

# Shader Compiler Tool RU

## Описание

Инструмент Компиляции Шейдеров может:
1. Компилировать файл HLSL и сохранять двоичный файл, содержащий байт-код, рефлексию и заголовок с информацией о шейдере.
2. Создавать кэш шейдеров в виде отдельных бинраных файлов для каждого шейдера в указанной директории.
3. Создавать единый файл шейдерного кэша, содержащий все скомпилированные шейдеры из указанной директории.
4. Компилировать указанную пару шейдера и метафайла и сохранять результат компиляции в единый файл шейдерного кэша.
5. Встраивать отладочную информацию в байт-код шейдера для упрощения отладки.
6. Генерировать файлы PDB, содержащие символы отладки, в указанной директории для использования с инструментами отладки.

## Аргументы

| Аргумент | Название        | Описание                                                                     |
|----------|-----------------|------------------------------------------------------------------------------|
| `-o`     | `--out`         | **Обязательный.** Путь к папке для сохранения результата                     |
| `-s`     | `--shaders`     | **Обязательный.** Путь к папке с шейдерами или к одному шейдеру              |
| `-m`     | `--metafiles`   | **Обязательный.** Путь к папке с метафайлами или к одному метафайлу          |
| `-i`     | `--includes`    | Опционально. Список путей для поиска заголовков (.h или .hlsli) для шейдеров |
| `-c`     | `--cache`       | Опционально. Имя файла шейдерного кэша                                       |  
| `De`     | `--debug-embed` | Опционально. Встраивать отладочную информацию в байт-код шейдера             |
| `Do`     | `--debug-out`   | Опционально. Путь к директории для сохранения PDB файлов для отладки         |
| `h`      | `--help`        | Показать справочное сообщение и выйти                                        |

## Использование

```sh
ShaderCompilerTool.exe -o <output_directory> -s <shaders_path> -m <metafiles_path> [-i <include_path1> <include_path2> ...] [-c <shader_cache_name>] [-Do <pdb_output_directory>] [-De]
```

## Требования

Для каждого файла HLSL должен быть метафайл .blk, который хранит в себе информацию об этапе или этапах, для которых
будет компилироваться шейдер (вершинный, пиксельный и т.д.), и информацию о пермутациях и дефайнах для каждой. Должна
быть как минимум одна пермутация - `Regular` (с дефайнами или без). Если есть другие пермутации и `Regular` не нужна, она
может отсутствовать. Также минимум один этап шейдера должен быть указан.
Все файлы могут находиться в разных папках или в одной. Метафайл должен иметь то же имя, что и шейдер.

## Как работает

Если оба пути указаны как файлы (шейдер и метафайл), будет компилироваться только эта пара. Все скомпилированные
варианты будут сохранены в один файл шейдерного кэша, который будет назван в соответствии с параметром `-с` или, если этот
параметр не указан, будет использовано имя шейдера.

Если оба пути являются папками, инструмент собирает пути ко всем метафайлам и HLSL файлам, затем сопоставляет их по имени.
Затем он компилирует каждый HLSL файл согласно его метаданным.
Скомпилированные файлы сохраняются во временную папку (которая будет удалена после завершения).
Когда все файлы успешно скомпилированы, инструмент либо создает отдельные бинарные файлы для каждого шейдера в выходной
директории, либо создает единый файл кэша шейдеров, в зависимости от того, указан ли `-c` или `--cache`.

Если используется флаг `-De` или `--debug-embed`, инструмент встроит отладочную информацию в байт-код шейдера, что упростит
отладку шейдеров. Если используется флаг `-Do` или `--debug-out`, инструмент выведет файлы PDB, содержащие символы 
отладки, в указанную директорию. Эта директория будет использоваться для инструментов отладки, предоставляя дополнительный 
контекст в процессе разработки. Флаги `-De` и `-Do` могут использоваться вместе. Когда оба флага указаны, инструмент 
встроит отладочную информацию в байт-код шейдера и также создаст отдельные файлы PDB для дополнительного контекста отладки.

Если компиляция или создание кэша завершаются ошибкой, процесс останавливается и отображается сообщение об ошибке. 
Если успешно, выводится сообщение об успешном завершении и полный путь к файлу кэша шейдеров или директории.
