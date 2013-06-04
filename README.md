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
Sample images were rendered with 15 iterations, patch size 5.0 and 5000 rays per patch.
####Sample Cornell Box scene
<p align="center">
<img align="center" src="https://raw.github.com/AlexanderTolmachev/radiosity-engine/master/images/sample-it15-s5.0-r5000.png"/>
</p>
####Sample scene with mesh model
<p align="center">
<img align="center" src="https://raw.github.com/AlexanderTolmachev/radiosity-engine/master/images/model-it15-s5.0-r5000.png"/>
</p>
