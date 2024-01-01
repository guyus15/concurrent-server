# Final Year Project

## Building
The project uses Premake to provide as a build system to provide configurations for each build target.
Download here: https://premake.github.io/

### Windows
1. Recursively clone the repository and enter the folder.
   ```
   git clone --recursive https://gitlab.cim.rhul.ac.uk/zjac142/PROJECT.git [CLONED NAME]
   cd [CLONED NAME]
   ```
2. Generate project files with Premake. This example assumes you are using Visual Studio 2022, but you can use any of the other generators available with Premake.
    ```
    premake5 vs2022
    ```
3. Open the generated solution file.
4. In Visual Studio, selected Build > Batch Build and select the desired project configurations, then click Build.
![Batch Build](docs/batch-build.png)

### Linux
1. Recursively clone the repository and enter the directory.
```
git clone --recursive https://gitlab.cim.rhul.ac.uk/zjac142/PROJECT.git [CLONED NAME]
cd [CLONED NAME]
```
2. Install the necessary prerequisite libraries to build the project.
```
sudo apt install libx11-dev
sudo apt install libxcursor-dev
sudo apt install libxrandr-dev
sudo apt install libxinerama-dev
sudo apt install libxi-dev
sudo apt install libgl-dev
```
3. Generate the project files with Premake. On Linux, this assumes you are using Make but you are free to use any of the other generators available with Premake.
4. Within the root directory of the project, run `make`. By default this will perform the `make all` command.
