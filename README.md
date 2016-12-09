# Point Clouds

Using an .obj file, this program will read a number of 3D point positions from the file, and render them onto the screen as a model as a point cloud implementation.

The model can be manipulated using these controls:

'x'/'X': move left/right (along the x axis) by a small amount

'y'/'Y': move down/up (along the y axis) by a small amount

'z'/'Z': move into/out of the screen (along the z axis) by a small amount

's'/'S': scale down/up (about the model's center, not the center of the screen)

'o'/'O': orbit the model about the window's z axis by a small number of degrees per key press, counterclockwise ('o') or clockwise ('O'). The z axis crosses the screen in the center of the window. 

'r': reset position, orientation and size

The program can be toggled between OpenGL mode and rasterizer mode using the 'm' key. 
A z-buffer algorithm is implemented, and pixels are dynamically resized based on their distance. 

bear.obj omitted due to file size.
