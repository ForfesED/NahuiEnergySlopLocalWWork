git clone https://github.com/microsoft/vcpkg 
call .\vcpkg\bootstrap-vcpkg.bat
.\vcpkg\vcpkg install --triplet=x64-windows
cmake -S . -B build 