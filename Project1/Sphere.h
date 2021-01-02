#pragma once
#ifndef SPHERE_H
#define SPHERE_H

struct point* PtsPointer(int, int);

void DrawPoint(struct point*);
void HsvRgb(float[3], float[3]);
void Axes(float);
void MjbSphere(float, int, int);
void SetMaterial(float, float, float, float);
void SetPointLight(int, float, float, float, float, float, float);
float* MulArray3(float, float[3]);
void SetMaterial(float, float, float, float);
void SetBlob(float, float, float);
void SetSpotLight(int, float, float, float, float, float, float, float, float, float);
#endif 
