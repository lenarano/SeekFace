#include "FindFace.h"

#define DEBUG false


FindFace::FindFace(QString s) : name(s)
{
	detector = dlib::get_frontal_face_detector(); //������ ��� ���������� ���� ��� �� �����������
	dlib::deserialize("shape_predictor_68_face_landmarks.dat") >> pose_model;//������ ��� ������������� �����������
	dlib::deserialize("dlib_face_recognition_resnet_model_v1.dat") >> net;//3.��������� DNN, ������������� �� ������������� ���.
}

std::vector<dlib::matrix<dlib::rgb_pixel>> FindFace::getfaces() { return faces; }

cv::Rect FindFace::getface() { return face; }

std::vector<dlib::matrix<float, 0, 1>> FindFace::descriptorFace()
{
	std::vector<dlib::matrix<float, 0, 1>> face_descriptors;
	//��������� ���������� �� ���������� ����
	face_descriptors = net(faces);
	return face_descriptors;
}

dlib::matrix<float, 0, 1> FindFace::descriptorFace1()
{
	std::vector<dlib::matrix<float, 0, 1>> face_descriptors;
	//��������� ���������� �� ���������� ����
	face_descriptors = net(faces);
	return face_descriptors[0];
}

dlib::matrix<float, 0, 1> FindFace::descriptorFace1(std::vector<dlib::matrix<dlib::rgb_pixel>> face)
{
	std::vector<dlib::matrix<float, 0, 1>> face_descriptors;
	//��������� ���������� �� ���������� ����
	face_descriptors = net(face);
	return face_descriptors[0];
}


void FindFace::detectorFace(cv::Mat im)//� ������������
{
	facesDlib.clear();
	faces.clear();
	shapes.clear();
	cv::Mat im_small;
	// ��������� ������� ����������� ��� ������������� ���
	cv::resize(im, im_small, cv::Size(), 1.0 / FACE_DOWNSAMPLE_RATIO, 1.0 / FACE_DOWNSAMPLE_RATIO);
	// �������� �� ������ ����������� dlib. ������ �� ����������.
	dlib::cv_image<dlib::bgr_pixel> cimg_small(im_small);
	dlib::cv_image<dlib::bgr_pixel> cimg(im);
	// ����������� ��� ��� ��������� ������� �����������
	facesDlib = detector(cimg_small);
	// ������� ���� ������� ����.
	for (unsigned long i = 0; i < facesDlib.size(); ++i)
	{
		// �������� ������ ����������� �������������� ��� ������� ���������� �����������.
		dlib::rectangle r(
			(long)(facesDlib[i].left() * FACE_DOWNSAMPLE_RATIO),
			(long)(facesDlib[i].top() * FACE_DOWNSAMPLE_RATIO),
			(long)(facesDlib[i].right() * FACE_DOWNSAMPLE_RATIO),
			(long)(facesDlib[i].bottom() * FACE_DOWNSAMPLE_RATIO)
		);
		// ����������� ���������� �� �������������� �����������
		dlib::full_object_detection shape = pose_model(cimg, r);
		shapes.push_back(shape);
		extract_image_chip(cimg, dlib::get_face_chip_details(shapes[i], 150, 0.25), face_chip);
		faces.push_back(std::move(face_chip));

		face = dlibRectangleToOpenCV(r);
		//imwrite("test1.jpg", im);
		cv::rectangle(im, face, cv::Scalar(0, 255, 0));//�������
		//imwrite("test2.jpg", im);
		if (DEBUG) {
			std::cout << "Number of faces: " << faces.size() << std::endl;
			render_face(im, shape);//68 �����
		}
			//imwrite("test3.jpg", im);
	}

}


std::vector<dlib::matrix<dlib::rgb_pixel>> FindFace::detectorFaceMaxOpim(cv::Mat im)//��� ����������� ��� ���������� �����������
{
	std::vector<dlib::matrix<dlib::rgb_pixel>> MyFaces;
	std::vector<dlib::rectangle> MyFacesDlib;
	std::vector<dlib::full_object_detection> MyShapes;
	MyFaces.clear();
	MyFacesDlib.clear();
	MyShapes.clear();
	dlib::cv_image<dlib::bgr_pixel> cimg(im);
	MyFacesDlib = detector(cimg);
	for (unsigned long i = 0; i < MyFacesDlib.size(); ++i)
	{
		MyShapes.push_back(pose_model(cimg, MyFacesDlib[i]));
		extract_image_chip(cimg, dlib::get_face_chip_details(MyShapes[i], 150, 0.25), face_chip);
		MyFaces.push_back(std::move(face_chip));
	}
	return MyFaces;
}


cv::Rect FindFace::dlibRectangleToOpenCV(dlib::rectangle r)
{
	return cv::Rect(cv::Point2i(r.left(), r.top()), cv::Point2i(r.right() + 1, r.bottom() + 1));
}

void FindFace::draw_polyline(cv::Mat &img, const dlib::full_object_detection& d, const int start, const int end, bool isClosed = false)
{
	std::vector <cv::Point> points;
	for (int i = start; i <= end; ++i)
	{
		points.push_back(cv::Point(d.part(i).x(), d.part(i).y()));
	}
	cv::polylines(img, points, isClosed, cv::Scalar(0, 255, 0), 2, 16);

}

void FindFace::render_face(cv::Mat &img, const dlib::full_object_detection& d)
{
	DLIB_CASSERT
	(
		d.num_parts() == 68,
		"\n\t Invalid inputs were given to this function. "
		<< "\n\t d.num_parts():  " << d.num_parts()
	);

	draw_polyline(img, d, 0, 16);           // ����� �������
	draw_polyline(img, d, 17, 21);          // ����� �����
	draw_polyline(img, d, 22, 26);          // ������ �����
	draw_polyline(img, d, 27, 30);          // ����������
	draw_polyline(img, d, 30, 35, true);    // ������ ���
	draw_polyline(img, d, 36, 41, true);    // ����� ����
	draw_polyline(img, d, 42, 47, true);    // ������ ����
	draw_polyline(img, d, 48, 59, true);    // �������� ����
	draw_polyline(img, d, 60, 67, true);    // ���������� ����

}



