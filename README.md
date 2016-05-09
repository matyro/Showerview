# Showerview
Particleshower viewer for a modified [Corsika](https://www.ikp.kit.edu/corsika/) simulation

The Software should run on any system with dedicated GPU - OpenGL 3.3+ and OpenCL Support (cl_khr_gl_sharing extension required)

Its currently under heavy development, so massiv changes can occur on daily basis.


Necessary libraries:
- [GLEW](https://github.com/nigels-com/glew)  (OpenGL loader)         [license](http://glew.sourceforge.net/credits.html)
- [GLFW](https://github.com/glfw/glfw)  (Window/Input Manager)  [license](http://www.glfw.org/license.html)
- [GLM](https://github.com/g-truc/glm)   (Vector/Matrix)         [license](http://glm.g-truc.net/0.9.7/glm-0.9.7.pdf)
- freetype2 (font loading)
- [FreeImage](http://freeimage.sourceforge.net/) (Texture loading)
- [OpenCL](https://www.khronos.org/opencl/)(Internal rendering)
