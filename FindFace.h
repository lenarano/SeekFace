#ifndef FINDFACE_H
#define FINDFACE_H

#define FACE_DOWNSAMPLE_RATIO 2


#include <QMainWindow>
#include <iostream>

#include <dlib/opencv.h>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>
#include <dlib/gui_widgets.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <dlib/dnn.h>
#include <dlib/clustering.h>
#include <dlib/string.h>
#include <dlib/image_io.h>



template <template <int, template<typename>class, int, typename> class block, int N, template<typename>class BN, typename SUBNET>
using residual = dlib::add_prev1<block<N, BN, 1, dlib::tag1<SUBNET>>>;

template <template <int, template<typename>class, int, typename> class block, int N, template<typename>class BN, typename SUBNET>
using residual_down = dlib::add_prev2<dlib::avg_pool<2, 2, 2, 2, dlib::skip1<dlib::tag2<block<N, BN, 2, dlib::tag1<SUBNET>>>>>>;

template <int N, template <typename> class BN, int stride, typename SUBNET>
using block = BN<dlib::con<N, 3, 3, 1, 1, dlib::relu<BN<dlib::con<N, 3, 3, stride, stride, SUBNET>>>>>;

template <int N, typename SUBNET> using ares = dlib::relu<residual<block, N, dlib::affine, SUBNET>>;
template <int N, typename SUBNET> using ares_down = dlib::relu<residual_down<block, N, dlib::affine, SUBNET>>;

template <typename SUBNET> using alevel0 = ares_down<256, SUBNET>;
template <typename SUBNET> using alevel1 = ares<256, ares<256, ares_down<256, SUBNET>>>;
template <typename SUBNET> using alevel2 = ares<128, ares<128, ares_down<128, SUBNET>>>;
template <typename SUBNET> using alevel3 = ares<64, ares<64, ares<64, ares_down<64, SUBNET>>>>;
template <typename SUBNET> using alevel4 = ares<32, ares<32, ares<32, SUBNET>>>;

using anet_type = dlib::loss_metric<dlib::fc_no_bias<128, dlib::avg_pool_everything<
	alevel0<
	alevel1<
	alevel2<
	alevel3<
	alevel4<
	dlib::max_pool<3, 3, 2, 2, dlib::relu<dlib::affine<dlib::con<32, 7, 7, 2, 2,
	dlib::input_rgb_image_sized<150>
	>>>>>>>>>>>>;


class FindFace :public QMainWindow
{
	Q_OBJECT

public:
	FindFace(QString name);

	cv::Rect getface();
	std::vector<dlib::matrix<dlib::rgb_pixel>> getfaces();
	std::vector<dlib::matrix<float, 0, 1>> descriptorFace();
	std::vector<dlib::matrix<dlib::rgb_pixel>> detectorFaceMaxOpim(cv::Mat im);
	dlib::matrix<float, 0, 1> FindFace::descriptorFace1();
	dlib::matrix<float, 0, 1> FindFace::descriptorFace1(std::vector<dlib::matrix<dlib::rgb_pixel>> faces);

	public slots:
	void detectorFace(cv::Mat im);
	
private:
	cv::Rect face;
	std::vector<dlib::matrix<dlib::rgb_pixel>> faces;
	QString name;
	dlib::frontal_face_detector detector;
	dlib::shape_predictor pose_model;
	std::vector<dlib::rectangle> facesDlib;
	std::vector<dlib::full_object_detection> shapes;
	dlib::matrix<dlib::rgb_pixel> face_chip;
	cv::Rect dlibRectangleToOpenCV(dlib::rectangle r);
	void draw_polyline(cv::Mat &img, const dlib::full_object_detection& d, const int start, const int end, bool isClosed);
	void render_face(cv::Mat &img, const dlib::full_object_detection& d);
	anet_type net;

};

#endif