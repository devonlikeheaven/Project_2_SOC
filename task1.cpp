#include <opencv2/opencv.hpp>
using namespace cv;
//int main() {
int task1_main() {
	Mat image = imread("coins.jpg", IMREAD_GRAYSCALE);
	if (image.empty()) {
		printf("couldnt read input image.\n");
		return -1;
	}
	Mat edges;
	Canny(image, edges, 100, 200);
	imwrite("outImage_Limcangco.jpg", edges);
	imshow("Edge Detection", edges);
	waitKey(0);
	return 0;
}