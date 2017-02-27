/*******************************************************************
 Common Error Codes used accross the Anduino Library.
 
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


#ifndef __ANDUINO_ERR_CODES_H
#define __ANDUINO_ERR_CODES_H

typedef enum Status
{
	STATUS_OK,
	STATUS_ERROR,
	STATUS_ERROR_ALREADY_INITIALISED,
	STATUS_IMAGE_FORMAT_NOT_SUPPORTED,
	STATUS_CRC_FAIL
} Status;


#endif /* __ANDUINO_ERR_CODES_H */