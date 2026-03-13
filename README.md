# Practice

## Overview

Summer internship 2025

## Quick Start

### Clone the repository into a folder named 'project'
```bash
git clone https://github.com/HubbleOS/DevTools.git project && cd project
```

## Makefile Targets

| Command      | Description                                                                                                    |
| ------------ | -------------------------------------------------------------------------------------------------------------- |
| `make`       | Build the project (default target, same as `make build`).                                                      |
| `make init`  | Initialize the project structure by creating necessary directories and basic source files if they don't exist. |
| `make build` | Compile the source code and build the executable.                                                              |
| `make test`  | Build and run the test executable located in the `test` directory.                                             |
| `make run`   | Run the compiled main executable.                                                                              |
| `make clean` | Remove all build artifacts and output files from the build directory (`build`).                                |
