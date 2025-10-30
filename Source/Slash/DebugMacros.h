#pragma once

#include "DrawDebugHelpers.h"

/*
* Draw Debug Macros for different shapes
* DEFAULT variants last indefinitely
* SINGLE_FRAME variants last for a single frame only

Some constant parameters:
* SPHERE_RADIUS: radius of debug sphere
* SPHERE_SEGMENTS: number of segments for debug sphere
* LINE_THICKNESS: thickness of debug lines
* POINT_SIZE: size of debug points
*/

inline constexpr float SPHERE_RADIUS = 25.f;
inline constexpr int32 SPEHERE_SEGMENTS = 12;
inline constexpr float LINE_THICKNESS = 2.f;
inline constexpr float POINT_SIZE = 15.f;

#define DRAW_SPHERE(Location) if (GetWorld()) \
{\
	DrawDebugSphere(GetWorld(), Location, SPHERE_RADIUS,  SPEHERE_SEGMENTS, FColor::Red, true); \
}\

#define DRAW_SPHERE_SINGLE_FRAME(Location) if (GetWorld()) \
{\
	DrawDebugSphere(GetWorld(), Location, SPHERE_RADIUS, SPEHERE_SEGMENTS, FColor::Red, false, -1.f, 0U, LINE_THICKNESS); \
}\

#define DRAW_LINE(StartLocation, EndLocation) if(GetWorld()) \
{\
	DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, true, 0U, LINE_THICKNESS); \
}\

#define DRAW_LINE_SINGLE_FRAME(StartLocation, EndLocation) if(GetWorld()) \
{\
	DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, false, -1.f, 0U, LINE_THICKNESS); \
}\
	
#define DRAW_POINT(Location) if(GetWorld()) \
{\
	DrawDebugPoint(GetWorld(), Location, POINT_SIZE, FColor::Red, true); \
}\

#define DRAW_POINT_SINGLE_FRAME(Location) if(GetWorld()) \
{\
	DrawDebugPoint(GetWorld(), Location, POINT_SIZE, FColor::Red, false, -1.f, 0U); \
}\

#define DRAW_VECTOR(StartLocation, EndLocation) if(GetWorld()) \
{\
	DRAW_POINT(EndLocation); \
	DRAW_LINE(StartLocation, EndLocation); \
}\

#define DRAW_VECTOR_SINGLE_FRAME(StartLocation, EndLocation) if(GetWorld()) \
{\
	DRAW_POINT_SINGLE_FRAME(EndLocation); \
	DRAW_LINE_SINGLE_FRAME(StartLocation, EndLocation); \
}\



