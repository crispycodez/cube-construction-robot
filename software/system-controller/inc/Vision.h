#pragma once

#include <QObject>
#include "opencv2/opencv.hpp"
#include "Logger.h"
/*!
* State-based computer vision interface for the robot
*/
class Vision : public QObject
{
	Q_OBJECT
public:
	/*!
	* Class constructor.
	* @param [in] parent Parent object.
	*/
	Vision(QObject* parent = Q_NULLPTR);

	/*!
	* Initialize vision system extrinsic rotation and translation matrices through the use of reference fiducials.
	*/
	void calibrate(const cv::Mat& image);

	/*!
	* Annotate image with fiducial information.
	* \param [in] image Image to add fiducial information to.
	*/
	void plotFiducialInfo(cv::Mat& image);

	/*!
	* Annotate image with cube information.
	* \param [in] image Image to add cube information to.
	*/
	void plotCubeInfo(cv::Mat& image);

	/*!
	* Annotate image with the computer vision bounding box.
	* \param [in] image Image to add bounding box to.
	*/
	void plotBoundingBox(cv::Mat& image);

	/*!
	* Compute the corresponding world point given an image point and the Z coordinate of the world point.
	* \param [in] imagePoint Coordinates of the source point in the uv image frame.
	* \param [in] z Z coordinate of the correspoding world point.
	* \return World point with specified z coordinate corresponding to given image point.
	*/
	cv::Point3i projectImagePoint(const cv::Point2d& imagePoint, double z) const;

	/*!
	* Compute the corresponding image point given a world point.
	* \param [in] worldPoint Coordinates of the source point in the XYZ world frame.
	* \return Image point corresponding to given world point.
	*/
	cv::Point projectWorldPoint(const cv::Point3d& worldPoint) const;

signals:
	/*!
	* Generated when a message is logged by an \class Vision instance.
	*/
	void log(Message message) const;

private:
	/*!
	* Collection of properties associated with a fiducial contour in the image frame.
	*/
	struct FiducialContour
	{
		int id; /*! Unique fiducial identifier as encoded in fiducial pattern */
		cv::Point centroid;
		std::vector<cv::Point> contour; /*! Collection of points describing contour around fiducial */
		std::vector<cv::Point> corners; /*! Set of four corners of the fiducial square in an anti-clockwise direction */
		cv::Mat homographyMatrix; /*! Homography matrix mapping fiducials from calibration image to isolated image */
	};

	/*!
	* Collection of properties associated with a cube contour in the image frame.
	*/
	struct CubeContour
	{
		std::vector<cv::Point> contour; /*! Collection of points describing contour around cube */
		std::vector<cv::Point> corners; /*! Set of four corners of the cube top-face in an anti-clockwise direction */
	};

	// Intrinsic camera parameters
	double fx = 696.2920653066839 * 2; /*! Camera x-axis focal length */
	double fy = 696.1538823160478 * 2; /*! Camera y-axis focal length */
	double cx = 469.7644569362635 * 2; /*! Camera principal point x-coordinate */
	double cy = 281.0969237061734 * 2; /*! Camera principal point y-coordinate */

	cv::Mat cameraMatrix; /*! Intrinsic camera matrix */
	cv::Mat distCoeffs; /*! Camera distorition coefficients */
	cv::Mat rotationMatrix; /*! Rotation matrix for world frame with respect to camera frame */
	cv::Mat translationVector; /*! Translation matrix for world frame with respect to camera frame */
	std::vector<FiducialContour> fiducialContours; /*! Set of fiducials identified in the image frame */
	std::vector<CubeContour> cubeContours;  /*! Set of cube contours in the image frame */
	QMap<int, cv::Point3i> fiducialWorldPoints; /*! Position in the world frame of each fiducial used */
	bool calibrated = false; /*! Flag to indicate if the vision system has been calibrated with a valid extrinsic matrix */
	cv::Point3i boundingBoxCorners[4]; /*! Coordinates of bounding box corners for computer vision system in the world frame */

	/*!
	* Get the contour centroid.
	* \param [in] contour Contour to find centroid from.
	* \return Centroid point of contour.
	*/
	cv::Point getCentroid(const std::vector<cv::Point>& contour) const;

	/*!
	* Assumes the input contour is a square and estimates the square corners.
	* \param [in] contour Contour to estimate corners from.
	* \return Estimated coordinates for 4 corners running clockwise around the square.
	*/
	std::vector<cv::Point> findSquareCorners(const std::vector<cv::Point>& contour) const;

	/*!
	* Isolate rectangle describes by corners in image space and warp to image.
	* \param [in] corners Four corners of rectangle in image space.
	* \param [in] sourceImage Image in which the rectangle is located.
	* \param [out] isolatedImage Image to which the rectangle is warped.
	* \param [out] homographyMatrix Homography matrix used to warp perspective from source image to isolated image.
	*/
	void isolateRectangle(const std::vector<cv::Point>& corners, const cv::Mat& sourceImage, cv::Mat& isolatedImage, cv::Mat& homographyMatrix) const;

	/*!
	* Find the identifier of an isolated fiducial image.
	* \param inputImage Input fiducial image.
	* \param outputImage Correctly oriented fiducial image.
	* \return Fiducial identifer. Returns -1 if not a valid fiducial.
	*/
	int processFiducial(const cv::Mat& inputImage, cv::Mat& outputImage) const;

	/*!
	* Map angle to the range (-PI, PI] randians.
	* \param [in] Angle to map in radians.
	* \return Mapped angle in radians.
	*/
	float mapAngle(float angle) const;
};