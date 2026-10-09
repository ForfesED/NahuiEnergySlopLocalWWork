# Material Creation Tool EN

## Description

The tool is designed to generate a material asset in JSON format based on specified shader assets. All material
properties are initialized with zero values (according to their type) and you need to edit these values directly in the
file.

## Arguments

| Key  | Full Key     | Description                                                                                                       |
|------|--------------|-------------------------------------------------------------------------------------------------------------------|
| `-o` | `--out`      | **Required.** Specify the material file to be created                                                             |
| `-c` | `--cache`    | **Required**. Path to the shader cache                                                                            | 
| `-p` | `--pipeline` | **Required.** Specify a pipeline name followed by a list of shader names; can be repeated for multiple pipelines  |
| `-h` | `--help`     | Display help message and exit                                                                                     |

## Usage
```shell
MaterialCreationTool.exe -o <material_file> -c <shader_cache_path> -p <pipeline_name1> <shader_name1> <shader_name2> [-p <pipeline_name2> <shader_name3> <shader_name4> ...]

```

## Requirements

There are no special requirements. You can specify any number of pipelines, each with any quantity and combination of
shaders, and the material will be generated accordingly. The tool does not validate the material itself. Please note
that you must provide the path to an existing shader cache and the names of assets within that cache. Source files (
.hlsl) and their names are not accepted by the tool.

## How it works

The tool retrieves the reflection of the specified shaders from the shader cache. Based on this reflection, it creates a
JSON file containing only the properties specific to the material itself. These include any constant buffers (except for
`b0`), textures, samplers, and other properties relevant to the material. Each property is saved with just its name and an
initial value.

Once the file is generated and written, the tool completes its task and displays a success message. If an error occurs
during execution or if any argument is missing or incorrect, an error message is shown.

You can edit the values of properties in the generated file. However, changing the name of a property or any associated
information might cause the material to malfunction, as the material relies on the property names and structure defined
by the shader reflection.

# Material Creation Tool RU

## Описание

Инструмент предназначен для создания материала в формате JSON на основе указанных шейдеров. Все свойства материала
инициализируются нулевыми значениями (в зависимости от их типа), и вам нужно редактировать эти значения непосредственно
в файле.

## Аргументы

| Key  | Full Key     | Description                                                                                                         |
|------|--------------|---------------------------------------------------------------------------------------------------------------------|
| `-o` | `--out`      | **Обязательный**. Укажите файл материала, который нужно создать                                                     |
| `-c` | `--cache`    | **Обязательный**. Путь к кэшу шейдеров                                                                              | 
| `-p` | `--pipeline` | **Обязательный**. Имя пайплайна, за которым следует список имен шейдеров; можно повторить для нескольких пайплайнов |
| `h`  | `--help`     | Показать сообщение помощи и выйти                                                                                   |

## Использование

```shell
MaterialCreationTool.exe -o <material_file> -c <shader_cache_path> -p <pipeline_name1> <shader_name1> <shader_name2> [-p <pipeline_name2> <shader_name3> <shader_name4> ...]
```

## Требования

Нет особых требований. Вы можете указать любое количество пайплайнов, каждый из которых может содержать любое количество
и комбинации шейдеров, и материал будет создан соответственно. Инструмент не выполняет валидацию материала.
Обратите внимание, что вы должны указать путь к существующему кешу шейдеров и имена ассетов в этом кеше. Исходные
файлы (.hlsl) и их имена инструмент не принимает.

## Как работает

Инструмент извлекает рефлексию указанных шейдеров из кеша шейдеров. На основе этой рефлексии создается JSON-файл,
содержащий только свойства, специфичные для самого материала. Это могут быть любые константные буферы (кроме b0),
текстуры, сэмплеры и другие свойства, относящиеся к материалу. Каждое свойство сохраняется в виде имя-значение.

После генерации и записи файла инструмент завершает свою работу и отображает сообщение об успешном завершении. Если во
время выполнения произойдет ошибка или если какой-либо аргумент отсутствует или указан неверно, будет показано сообщение
об ошибке.

Вы можете редактировать значения свойств в созданном файле. Однако изменение имени свойства или любой связанной
информации может привести к тому, что материал перестанет функционировать, поскольку материал зависит от имен и
структуры свойств, определенных рефлексией шейдера.
