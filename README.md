# irFlip2
Reimplemented as a more general utility, it now takes an input argument to specify swizzle for rearranging channels.

Swizzle syntax consists of 4 characters, from the set { R, r, G, g, B, b, A, a }, where uppercase is the input channel value, and lowercase is 255 minus the input channel value, e.g. value invert.
