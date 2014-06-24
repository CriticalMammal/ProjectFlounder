//FILE : camera.h
//PROG : Dylan Gallardo
//PURP : header for the camera source file. Many functions/variables are present to
//		 handle the movement of the camera and determine when and how to move.

#ifndef CAMERA_H
#define CAMERA_H

class Camera
{
	private:
		double vx, vy,speed, maxSpeed, friction, cameraPause,
			zoomVelocity, zoomToPoint;
		int width, height, moveToPointX, moveToPointY,
			accuracy, motion;
	public:
		Camera();
		~Camera();
		void handleKeys(double playerSpeed);
		void scrollScreen();
		void newMoveToPoint(Sprite *sprite);
		void newZoom(double);
		double randomNumber(double Min, double Max);

		//quick inline functions
		double getSpeed() {return speed;}
		double getCameraPause() {return cameraPause;}
		int   getAccuracy() {return accuracy;}
		int   getMotion() {return motion;}
		int   getMoveToPointX() {return moveToPointX;}
		int   getMoveToPointY() {return moveToPointY;}

		void setSpeed(double newSpeed) {speed = newSpeed;}
		void setCameraPause(double newPause) {cameraPause = newPause*FPS;}
		void setAccuracy(int newAccuracy) {accuracy = newAccuracy;}
		void setMotion(int newMotion) {motion = newMotion;}
};

#endif