# Final Year Project

## Building
### Windows
The project uses Premake to provide as a build system to provide configurations for each build target.
Download here: https://premake.github.io/

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
To be added...