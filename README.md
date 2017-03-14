# Raytracer
Implementation of raytracer algorithm 

## Summary
It's a study project (with 42 coursus) about implementation of basic raytracing algorithm.
As a bonus on top: real-time raytracing supported.

See more info in pdf "RTv1.en.pdf"

It is a representation of how we percieve a physical reality with simple 3d objects, lights and colors.

### Prerequisites
Current version runs only on iMac systems (due to minilibx and it's Makefile build).

### Installation

To compile run make command in your terminal (while in the folder):
```
make
```

After that simply run ./RTv1 with an argument that takes a path to a config file:
```
././RTv1 [path_to_config/config_name]
```
For example
```
././RTv1 configs/lights_ultimate
```

### Inside the raytracer

Once run you'll see rendered 3d objects specified in config file.

Controls displayed in your terminal for real-time raytracing (~2-4 FPS):
movement with arrows (left-right, up-down) + 4,1 (Nums for depth), as well as rotations: 5,2 Nums and 6,3 Nums 

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details

## Acknowledgments

* Arthur Appel
* Jim Kajiya
