// color_util.h
/* Copyright © 2005–2010, Pascal Getreuer 
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are
 * permitted provided that the following conditions are met:
 *
 * 	1. Redistributions of source code must retain the above copyright notice, this list of
 * 	   conditions and the following disclaimer.
 *
 * 	2. Redistributions in binary form must reproduce the above copyright notice, this list
 * 	   of conditions and the following disclaimer in the documentation and/or other materials
 * 	   provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY <COPYRIGHT HOLDER> ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
// Thank you, Pascal Getreuer!
#include <stdint.h>
#include <SDL.h>

// TODO: Uint32_t support? After all, our SDL_Surfaces are 32-bit...
typedef struct {
	uint8_t r, g, b;
} color;

// TODO: Uint32_t support? After all, our SDL_Surfaces are 32-bit...
color get_color(SDL_Surface * img, int x, int y) {
	color rgb;
	uint8_t r, g, b, pixel;
	uint8_t * pixels = (uint8_t *) img->pixels;
	pixel = pixels[(y * img->w) + x];
	SDL_GetRGB(pixel, img->format, &rgb.r, &rgb.g, &rgb.b);
	return rgb;
}

// From: http://www.math.ucla.edu/~getreuer/colorspace.html
void Rgb2Hsl(float * H, float * S, float * L, float R, float G, float B) {
	float Max = max(R, G, B);
	float Min = min(R, G, B);
	float C = Max - Min;
	*L = (Max + Min)/2.0;
	if(C > 0) {
		if(Max == R) {
			*H = (G - B) / (float) C;
			if(G < B)
				*H += 6;
		}
		else if (Max == G) {
			*H = 2.0 + (B - R) / (float) C;
		}
		else {
			*H = 4.0 + (R - G) / (float) C;
		}
		*H *= 60.0;
		*S = (*L <= 0.5) ? (C / (2.0 * (*L))) : (C / (2.0 - 2.0 * (*L)));
	}
	else {
		*H = *S = 0;
	}
	*H /= 360.0;
}

// From: http://www.math.ucla.edu/~getreuer/colorspace.html
void Hsl2Rgb(float * R, float * G, float * B, float H, float S, float L) {
	float C = (L <= 0.5) ? (2 * L * S) : ((2 - 2 * L) * S);
	float Min = L - 0.5 * C;
	float X;
	H *= 360.0;	
	H -= 360.0 * floor(H / 360.0);
	H /= 60.0;
	X = C * (1 - fabs(H - 2 * floor(H / 2.0) - 1));
	switch((int) H)
	{
		case 0:
			*R = Min + C;
			*G = Min + X;
			*B = Min;
			break;
		case 1:
			*R = Min + X;
			*G = Min + C;
			*B = Min;
			break;
		case 2:
			*R = Min;
			*G = Min + C;
			*B = Min + X;
			break;
		case 3:
			*R = Min;
			*G = Min + X;
			*B = Min + C;
			break;
		case 4:
			*R = Min + X;
			*G = Min;
			*B = Min + C;
			break;
		case 5:
			*R = Min + C;
			*G = Min;
			*B = Min + X;
			break;
		default:
			*R = *G = *B = 0;
	}
}

void Sound2Hsl(float * H, float * S, float * L, float f, float a, long scale, int l) {
	*H = (f - l) / scale;
	*S = 1;
	*L = a;
}

