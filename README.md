
# Cerber Project

## Overview

The Cerber Project is a multi-functional server application written in C. It serves static HTML files, executes Java programs, and runs a Raylib graphical application. This project demonstrates how to integrate various components and technologies into a single server application.

## Features

- Serve static HTML pages.
- Execute Java programs and display their output.
- Run a Raylib graphical application.

## Dependencies

### Required Software

- GCC (GNU Compiler Collection)
- Java Development Kit (JDK)
- Raylib (for graphical applications)
- Windows OS or compatible Unix-based OS

### Installing Dependencies on Windows

1. **GCC:** Install via MSYS2
    - Download and install MSYS2 from [msys2.org](https://www.msys2.org/).
    - Update package database and core system packages:
      ```sh
      pacman -Syu
      ```
    - Install GCC:
      ```sh
      pacman -S mingw-w64-x86_64-gcc
      ```

2. **Java Development Kit (JDK):**
    - Download and install the JDK from [Oracle](https://www.oracle.com/java/technologies/javase-jdk11-downloads.html) or [OpenJDK](https://openjdk.java.net/install/).

3. **Raylib:**
    - Download Raylib from [raylib.com](https://www.raylib.com/) and follow the installation instructions.

## Project Structure

```
Cerber
|-- bin
|   |-- HelloWorld.class
|   |-- server.exe
|   |-- raylib_app.exe
|
|-- build
|   |-- build.c
|   |-- build.exe
|
|-- include
|   |-- controller.h
|   |-- model.h
|   |-- view.h
|
|-- public
|   |-- customers.html
|   |-- index.html
|   |-- orders.html
|
|-- src
|   |-- controller.c
|   |-- HelloWorld.java
|   |-- main.c
|   |-- model.c
|   |-- raylib_app.c
|   |-- view.c
```

## Setup Instructions

### Step 1: Clone the Repository

```sh
git clone https://github.com/chuvpylov/cerber.git
cd cerber
```

### Step 2: Build the Project

Navigate to the `build` directory and compile the build script:

```sh
cd build
gcc -o build build.c
./build.exe
```

### Step 3: Compile the Java Program

Ensure the Java program is compiled and the class file is placed in the `bin` directory:

```sh
javac -d ../bin ../src/HelloWorld.java
```

### Step 4: Run the Server

Run the server executable from the `bin` directory:

```sh
../bin/server.exe
```

## Accessing the Application

Open a web browser and navigate to the following URLs:

- **Home Page:** `http://localhost:8080`
- **Java Program:** `http://localhost:8080/runjava`
- **Raylib Application:** `http://localhost:8080/runraylib`

## Troubleshooting

### Common Issues

1. **Missing Dependencies:**
    - Ensure all dependencies are installed and properly configured.

2. **Build Errors:**
    - Verify the paths in the build script and the server code.
    - Ensure the environment variables and paths are set correctly.

3. **Server Not Starting:**
    - Check for any error messages in the console.
    - Ensure the required ports are not in use by other applications.

## Contributing

We welcome contributions to enhance the Cerber Project. Please fork the repository and submit pull requests for any improvements or bug fixes.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.
