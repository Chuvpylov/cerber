
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

### Installing Dependencies on Linux

#### GCC
```bash
sudo apt update
sudo apt install build-essential
```

#### Java Development Kit (JDK)
```bash
sudo apt install default-jdk
```

#### Raylib
```bash
sudo apt install libraylib-dev
```

## Project Structure

```
Cerber
|-- bin
|   |-- HelloWorld.class
|   |-- server
|   |-- raylib_app
|
|-- build
|   |-- build.c
|   |-- build
|
|-- include
|   |-- controller.h
|   |-- log_manager.h
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
|   |-- log_manager.c
|
|-- test
|   |-- test_main.c
|
|-- .gitignore
|-- README.md
```

## Setup Instructions

### Step 1: Clone the Repository

```bash
git clone https://github.com/chuvpylov/cerber.git
cd cerber
```

### Step 2: Build the Project

Navigate to the build directory and compile the build script:

```bash
cd build
gcc -o build build.c
./build
```

### Step 3: Compile the Java Program

Ensure the Java program is compiled and the class file is placed in the `bin` directory:

```bash
javac -d ../bin ../src/HelloWorld.java
```

### Step 4: Run the Server

Run the server executable from the `bin` directory:

```bash
../bin/server
```

## Accessing the Application

Open a web browser and navigate to the following URLs:

- Home Page: `http://localhost:8080`
- Java Program: `http://localhost:8080/runjava`
- Raylib Application: `http://localhost:8080/runraylib`

## Troubleshooting

### Common Issues

#### Missing Dependencies

Ensure all dependencies are installed and properly configured.

#### Build Errors

Verify the paths in the build script and the server code.
Ensure the environment variables and paths are set correctly.

#### Server Not Starting

Check for any error messages in the console.
Ensure the required ports are not in use by other applications.

## Contributing

We welcome contributions to enhance the Cerber Project. Please fork the repository and submit pull requests for any improvements or bug fixes.

## License

This project is licensed under the MIT License. See the LICENSE file for details.
