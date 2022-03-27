# ofxKaleidoscope
Openframeworks addon to render a kaleidoscope from ofPixels input.

![Screenshot of example app](screenshot.jpg?raw=true "Screenshot of example app")

Based on code from Anthropocene -> https://github.com/HellicarAndLewis/Anthropocene

See /example project. Build it using Project Generator

Tested against OF 0.11.2 on Mac only.



### TODO

* Work out a fit/fill to width/height algorithm based on radius and segments.
* Add option to animate the offset - or do so in example, in order to make a still image more interesting
* Add alt. update method overloads to take different input types of Pixel data
* Add note about segments needing to be even, for perfect blend.
* On zoomed in, could blur edges to give a lot of depth, think an outer, radial blur, tilt shift / cataract type thing. - Shader?
 

