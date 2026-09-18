

# miniRT

## Description
**miniRT** is a 42 school project that serves as an introduction to the beautiful world of Ray Tracing. The goal of the project is to render simple 3D computer-generated images using the Ray-Tracing protocol. By calculating the intersection of light rays with basic geometric shapes, this program creates a realistic view of 3D objects—including spheres, planes, and cylinders. It applies ambient and spot lighting, handles shadows, and manages camera perspectives to simulate real-world physics and optics.

## Instructions

### Compilation
The project uses a `Makefile` to compile the source code into the executable program. 

To build the `minirt` executable, run:
```bash
make
```

You can also use standard Makefile rules to clean up build artifacts:
* `make clean` - Removes object files.
* `make fclean` - Removes object files and the compiled executable.
* `make re` - Recompiles the entire project from scratch.

### Installation & Dependencies
This project uses the **MLX42** graphics library. Ensure you have the required dependencies for MLX42 installed on your system (e.g., `glfw`). The MLX42 submodule should be initialized and built automatically by the Makefile, but if needed, you can initialize submodules via:
```bash
git submodule update --init --recursive
```

### Execution
Run the program by passing a scene description file (`.rt` extension) as the first argument. You can find example scene files in the `scenes/` directory.

```bash
./minirt <path_to_scene.rt>
```

**Example usage:**
```bash
./minirt scenes/cylinder.rt
```

## Features
* **Primitives:** Mathematical rendering of Spheres, Planes, and Cylinders.
* **Transformations:** Translation and rotation of objects, lights, and cameras.
* **Lighting Engine:** Supports ambient lightning, spot lights, and calculates hard shadows.
* **Window Management:** Smooth window rendering and event handling (ESC, window cross) via MLX42.

## Resources

### References
* [Ray Tracing in One Weekend](https://raytracing.github.io/books/RayTracingInOneWeekend.html) - The standard entry point for understanding the math and core concepts of ray tracing.
* [MLX42 Documentation](https://github.com/codam-coding-college/MLX42) - Documentation for the graphics library used to display the rendered pixels.

### AI Usage
* **Formatting:** AI was utilized to format hand-written code to fit the norm.
* **Documentation:** AI was utilized to draft and structure this README.md file in accordance with project requirements.
