This project was created by Matthew Martin for the ECS 175 (Computer Graphics) class in Winter of 2017.
This is the final project in the class - a Ray Tracer.

A Ray Tracer is a program that renders a scene by calculating the color of each screen pixel individually.
This is achieved by sending a "ray" from the pixel into the scene and calculating the color of an object
it encounters. If an object is hit, two more rays are sent from that position (one of reflection, and 
one for refraction), and added to the original intersection color. This allows for a simple recursive
algorithm to render a very detailed and accurate scene that contians objects or varying shape and physical
properties.

This project relies on OpenGL to initialize a drawing window and to draw a single pixel of a passed color.
All other functionality was written from scratch. This project was developed on Linux (Fedora 24).