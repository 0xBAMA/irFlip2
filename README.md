# irFlip2
Designed for channel swapping infrared images, now reimplemented as a more general utility, it takes an input argument to specify a swizzle for rearranging channels.

Swizzle syntax consists of 4 characters, from the set ```{ R, r, G, g, B, b, A, a }```, where uppercase is the input channel value, and lowercase is 255 minus the input channel value, e.g. value invert.

Example: 
```irFlip2 input.png RbgA```
