#pragma once

#include "Packet.h"
#include "Logger.h"
#include <QSerialPort>

/*!
* Defines an interface for interfacing with the robotic subsystem that abstracts the serial communication mechanism.
*/
class Robot: public QObject
{
	Q_OBJECT
public:
	/*!
	* Class constructor.
	* \param [in] parent Parent \class QObject.
	*/
	Robot(QObject* parent);

	/*!
	* Calibrate the reference positions for the X, Y and Z axes. The robot will perform a calibration sequence to find these positions using the
	* limit switches on each of the axes.
	*/
	void calibrate();

	/*!
	* Places all of the robotic subsystem motors in sleep mode. Note the robot must be recalibrated after performing this step.
	*/
	void sleep();

	/*!
	* Places all of the robotic subsystem motors in active mode.
	*/
	void wake();

	/*!
	* Set the serial port over which a connection with the robot has already been established.
	* \param[in] port Serial port 
	*/
	void setPort(QSerialPort* port);

	/*!
	* Instructs the robot to move the end effector to specified position and orientation.
	* \param [in] xPos X axis position (in horizontal steps)
	* \param [in] yPos Y axis position (in horizontal steps)
	* \param [in] zPos Z axis position (in vertical steps)
	* \param [in] rPos R axis (rotation about Z axis) position (in rotational steps)
	*/
	void setPosition(int xPos, int yPos, int zPos, int rPos);

	/*!
	* Getter for the x-axis stepper motor position in horizontal steps.
	*/
	int getXPosition();

	/*!
	* Getter for the y-axis stepper motor position in horizontal steps.
	*/
	int getYPosition();

	/*!
	* Getter for the z-axis stepper motor position in vertical steps.
	*/
	int getZPosition();

	/*!
	* Getter for the r-axis stepper motor position in rotational steps.
	*/
	int getRPosition();

	/*!
	* Getter for the internal vacuum system pressure reading.
	*/
	int getPressure();

	/*!
	* Set the vacuum actuator to ACTUATE positon. This generates a pressure lower than atmospheric pressure for the suction cup to grip an object.
	*/
	void actuateGripper();

	/*!
	* Set the vacuum actuator to RELEASE positon. This generates a pressure slightly higher than atmospheric pressure to ensure the object is
	* completely released.
	*/
	void releaseGripper();

	/*!
	* Set the vacuum actuator to the IDLE position. This is the default position of the actuator when not in the process of releasing or gripping an
	* object.
	*/
	void resetGripper();

	/*!
	* Request the internal pressure reading of the vacuum system. The robot will transmit a packet containing pressure sensor reading in response.
	*/
	void requestPressure();

	/*!
	* Pause briefly before continuing with next action
	*/
	void delay();

signals:
	/*!
	* Signal generated when robot indicates the most recent command has been completed.
	*/
	void commandCompleted() const;

	/*!
	* Signal generated when the robot returns a pressure reading update in response to the pressure request.
	*/
	void pressureUpdated() const;

	/*!
	* Generated when a message is logged by an \class Robot instance.
	*/
	void log(Message message) const;

private:
	QSerialPort* port = Q_NULLPTR; /*! Serial port for communication with robot */
	int xPos; /*! Robot x-axis motor step position */
	int yPos; /*! Robot y-axis motor step position */
	int zPos; /*! Robot z-axis motor step position */
	int rPos; /*! Robot r-axis motor step position */
	int pressure; /*! Pressure inside vaccum system updated on request */

	/*!
	* Transmit the packet over the serial port instance.
	* \param [in] packet Packet to transmit.
	*/
	void transmitPacket(const Packet &packet);

	/*!
	* Check if a packet has been received.
	*/
	void serialDataReceived();
};

