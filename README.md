# ECE 1400 Spring 2022 HW 11

## Additional instructions

Do not commit your data files or commpiled executables.  

You will need to get the STB library before you start this project.  First, enter the directory.

```
cd stb
```
Then you will need to initialize the submodule.
```
git submodule init
```
Then you will need to update the module. This will pull the code from the repository for you to use.
```
git submodule update
```
To use the module you must use this in the relevant file:
```C
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"
```

We will use the read function:
```C
int width, height, channels;
    
unsigned char *im_char = stbi_load(filename, &width,
	 	     	           &height, &channels, 0);

```
And the write function:
```C
stbi_write_jpg(filename, &width, &height,
    		   &channels, im_char, 100);

```
You don't need to use those variables.  This is just for the sake of example.