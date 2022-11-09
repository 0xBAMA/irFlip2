# irFlip2
Designed for channel swapping infrared images, now reimplemented as a more general utility, it takes an input argument to specify a swizzle for rearranging channels. Currently only operates on PNG compressed images, but at some point I'll add stb_image/stb_image_write to handle other formats.

Swizzle syntax consists of 4 characters, from the set ```{ R, r, G, g, B, b, A, a, 0, 1 }```, where uppercase is the input channel value, and lowercase is 255 minus the input channel value, e.g. value invert. Special values 0 and 1 to saturate to minimum or maximum value, respectively. There are 10000 ( choosing 4 elements from a set of 10, with replacement = 10x10x10x10 = 10000 ) valid combinations.

Example: 

```irFlip2 input.png RbgA```

This particular call to the program will take some image, ```input.png```, and produce an output which uses the red channel directly, inverts and swaps the blue and green channels, and uses the alpha channel directly.
