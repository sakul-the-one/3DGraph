#ifndef D3GRAPHICS_H
#define D3GRAPHICS_H
#define Debug true
#define PI 3.14159265358979323846
#define RadianToDegreee 180/PI
#define DegreeToRadian PI/180
#include <graphx.h>

typedef struct 
{
    float x;
    float y;
    float z;
} Vector3;

typedef struct 
{
    float x;
    float y;
} Vector2;

typedef struct 
{
    Vector2 pos1;
    Vector2 pos2;
} DrawDataLine; //We dont need for Point, since it is just a Vector 2

float fov = 90.0f;
Vector3 WorldRotation = {0,0,0};
Vector3 WorldPosition = {0,0,0};
//float aspectRatio = 320.0f / 240.0f;
//float near = 0.1f;
//float far = 100.0f;
bool D3G_SSD = false; //SaveScreenData

DrawDataLine * _DLD = NULL; //DrawLineData
Vector2 * _DPD = NULL; //DrawPointData
int _DLDCount = 0; //Remember to Count!
int _DPDCount = 0;


void D3G_SetWorldRotation(Vector3 NewRotation);
void D3G_AddWorldRotation(Vector3 NewRotation);
Vector3 D3G_GetWorldRotation();
void D3G_SetWorldPosition(Vector3 NewPosition);
Vector3 D3G_GetWorldPosition();

/** 
* @brief Initiates the libary
*/
void D3G_Init();

/** 
 * @brief Draws a Point on the screen
 *
 * This function projects a 3D point on the 2D Screen. The Null position is at the Center of the Screen
 * 
 * @param pos The 3D position of the Point 
*/
void D3G_DrawPoint(Vector3 pos);

/** 
 * @brief Draws a Line on the screen
 *
 * This function projects two 3D points on the 2D Screen and draws a line between them. The Null position is at the Center of the Screen.
 * 
 * @param pos1 The 3D position of the first Point.
 * 
 * @param pos2 The 3D position of the second Point.
*/
void D3G_DrawLine(Vector3 pos1,  Vector3 pos2);

/** 
 * @brief Dereferences the position
 *
 * You need to use this function in the end of the Programm, otherwise you may face graphical consequences
 * It also frees some important variables
*/
void D3G_Destroy();

/** 
 * @brief Draws a Cube on the screen
 *
 * Draws a Cube at the given position, size and rotation.
 * 
 * @param pos The 3D position of the Cube
 * @param size The size of the Cube. (Warning: a too small size [like 1], would make the Cube just some pixels big, or even just one pixel big)
 * @param rotation The Rotation the Cube should have
*/
void D3G_DrawCube(Vector3 pos, int8_t size, Vector3 rotation);

/** 
 * @brief Redraws everything saved.
 *
 * The Screen will be resetet and everything saved using the D3G_SetSSD(bool value) function will be redrawn.
 * 
*/
void D3G_Redraw();

/** 
 * @brief Saves everything drawn on the screen
 *
 * The Standart value of it is false, so it needs to be specifically turned on. Not recommended for using with Animations.
 * In every draw function, the position of the points will be saved into the ram, so it could be redrawn in the void D3G_Redraw() function.
 * 
 * @param value The value, the Variable should be set to
*/
void D3G_SetSSD(bool value);
/**
 * @brief Rotates a point in 3D space using specified rotation angles.
 *
 * This function applies a sequence of rotations around the X, Y, and Z axes
 * to a given point.
 *
 * @param point The point to be rotated.
 * @param rotation The angles in radians by which to rotate the point around the X, Y, and Z axes.
 * @return The rotated point.
 */
Vector3 D3G_RotatePoint(Vector3 point, Vector3 rotation);

/**
 * @brief Rotates a point in 3D space using specified rotation angles using a Privot
 *
 * This function applies a sequence of rotations around the X, Y, and Z axes
 * to a given point.
 *
 * @param point The point to be rotated.
 * @param rotation The angles in radians by which to rotate the point around the X, Y, and Z axes.
 * @param privot The privot to rotate around. For simplicity, just use the center of your object
 * @return The rotated point.
 */
Vector3 D3G_RotatePointNormalized(Vector3 point, Vector3 rotation, Vector3 privot);

#if Debug
void D3G_DrawDebugPoint(Vector3 pos);
void intToStr(int N, char *str);
#endif

Vector2 project(Vector3 point);
#endif