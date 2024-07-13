#pragma once
#include "Calibration.h"

class HandEyeCalibration :
	public Calibration
{
public:
	HandEyeCalibration(const std::string& imgsDirector,
		const std::string& outputFilename = "out_camera_data.yml",
		Size boardSize = Size(11, 8),
		double squareSize = 10.);  // mm or m

	~HandEyeCalibration();

public:
	// load camera extrinsic and disort coefficients matrix 
	// form file, eg. out_camera_data.yml
	static bool readCameraParameters(const std::string& outputFilename,
		cv::Mat& camMatrix, cv::Mat& distCoefs = Mat());

	// load extrinsic and attitude datas
	[[deprecated("use another override method instead of this one. \
		( Files data format must be specified.")]]
	static bool readDatasFromFile(const std::string& file,
		std::vector<cv::Mat>& vecHg, std::vector<cv::Mat>& vecHc);

	// load extrinsic datas from yml, eg. out_camera_data.yml, load camera extrinsic datas 
	// one-to-one correspondence 
	static bool readDatasFromFile(
		const std::string& file,
		const std::string& folder,
		std::vector<cv::Mat>& vecHg, std::vector<cv::Mat>& vecHc, bool useQuaternion = true);

	// calculate A��B
	static void convertVectors2Hij(
		std::vector<cv::Mat> & vecHg, std::vector<cv::Mat> & vecHc,
		std::vector<cv::Mat> & vecHgij, std::vector<cv::Mat> & vecHcij);

	// solve  Ax = xB
	static void computerHandEyeMatrix(Mat& Hcg, vector<Mat>& Hgij, vector<Mat>& Hcij, int method = 0,
		const std::string& file = "HandEyeMatrix.yml");

	static void Tsai_HandEye(Mat& Hcg, vector<Mat>& Hgij, vector<Mat>& Hcij); // private
	static void Navy_HandEye(Mat& Hcg, vector<Mat>& Hgij, vector<Mat>& Hcij); // private

	// attitude vector => matrix��  10 elems {x,y,z,}
	static cv::Mat attitudeVectorToMatrix(cv::Mat& m ,
		bool useQuaternion = true, const std::string& seq = "");
	
	// check rotatation matrix 3x3 or 4*4{r,t}
	static bool isRotationMatrix(const cv::Mat& R);

	// eulerAngle => rotated matrix
	static cv::Mat eulerAngleToRotatedMatrix(const Mat& eulerAngle, const std::string& seq = "zyx");

	// quaternion vector to matrix
	static cv::Mat quaternionToRotatedMatrix(const cv::Vec4d& q);

	// solve location
	static cv::Mat solveLocation(cv::Vec3d& pos, cv::Mat& Hcg, cv::Mat& Hg,
		cv::Mat& extParamMatrix, cv::Mat& camMatrix) = delete;

	/** @brief ����ͼ������(У��)����ڻ�����ϵ�µ�λ��
	*
	*	@note  ��������double������ֵ���ο�������ʹ�ã��ĵ�λ����̬����λһ��
	*
	*	@param 	imgPt				ͼ������ (u,v)
	*	@param 	z					�߶�
	*	@param 	Hcg					���۱궨�任���� 4*4
	*	@param 	Hg					ĩ����̬���� 4*4
	*	@param 	camMatrix			����ڲξ��� 3*3
	*	@param 	refHg				�ο���̬���󣨱�����
	*	@param 	refExtMatrix		�ο���ξ��󣨱�����
	*
	*	@return �ռ���ά������
	*/
	static cv::Point3d getWorldPos(cv::Point2d& imgPt, double z, cv::Mat& Hcg, cv::Mat& Hg,
		cv::Mat& camMatrix, cv::Mat& refHg = cv::Mat(), cv::Mat& refExtMatrix = cv::Mat());

	static std::vector<cv::Point3d> getWorldPos(
		std::vector<cv::Point2d>& imgPts, double z,
		cv::Mat& Hcg, cv::Mat& Hg, cv::Mat& camMatrix,
		cv::Mat& refHg = cv::Mat(), cv::Mat& refExtMatrix = cv::Mat()) = delete;
};