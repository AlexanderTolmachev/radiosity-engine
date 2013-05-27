Radiosity engine
================
About
-----
This is a simple radiosity engine written in C++ using Qt.

Usage: 
```
radiosity-engine.exe --scene=scene.xml --output=image.png --resolution_x=512 --resolution_y=512 --num_iterations=10 --patch_size=1.5 --num_rays_per_patch=5000
```

Sample images
-------------
####Sample Cornell Box scene
Rendered with 10 iterations, patch size 5.0 and 5000 rays per patch.
<p align="center">
<img align="center" src="https://raw.github.com/AlexanderTolmachev/radiosity-engine/master/images/sample-it10-s5-r5000.png"/>
</p>
