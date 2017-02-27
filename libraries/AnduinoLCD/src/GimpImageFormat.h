/*******************************************************************
 Contains typedef for images exported using GIMP
 
 Check out the included Arduino sketch examples and Anduino Wiki to
 get started!
 https://github.com/andium/Anduino/wiki
 
 Written by Dr James Dooley Copyright © 2016 Andium
 
 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at
 
 http://www.apache.org/licenses/LICENSE-2.0
 
 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 *******************************************************************/

#ifndef __GIMPIMAGES_H
#define __GIMPIMAGES_H

#include "stdint.h"


/* structure of images exported from GIMP as *.c */
typedef struct
{
	uint8_t   width;
	uint8_t   height;
	uint8_t   bytes_per_pixel; /* 2:RGB16, 3:RGB, 4:RGBA */ 
	char      *pixel_data;
} gimpImage;



#endif /* __GIMPIMAGES_H */