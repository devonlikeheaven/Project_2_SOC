#include <systemc>
#include <opencv2/opencv.hpp>
#include <filesystem>
using namespace sc_core;
using namespace std;
using namespace cv;

// initiator
SC_MODULE(Initiator) {
    sc_fifo_out<Mat> out;
    void run() {
        cout << "Initiator thread running..." << endl;
        Mat img = imread("road.jpg");//issues finding image
        if (img.empty()) {
            sc_stop();
            return;
        }
        out.write(img);
    }
    SC_CTOR(Initiator) {
        SC_THREAD(run);
    }
};

//target
SC_MODULE(Target) {
    sc_fifo_in<Mat> in;

    void run() {
        Mat img = in.read(); 
        Mat gray, edges;
        cvtColor(img, gray, COLOR_BGR2GRAY);
        Canny(gray, edges, 50, 150);
        vector<Vec4i> lines;
        HoughLinesP(edges, lines, 1, CV_PI / 180, 50, 50, 10);
        for (const auto& l : lines) {
            line(img, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 0, 255), 2);
        }
        imwrite("detectedLines_Limcangco.jpg", img);
        imshow("Detected Lines", img);
        waitKey(0);
    }
    SC_CTOR(Target) {
        SC_THREAD(run);
    }
};

//systemc main
int sc_main(int argc, char* argv[]) {
    sc_fifo<Mat> channel(1); //FIFO buffer
    Initiator initiator("Initiator");
    Target target("Target");
    initiator.out(channel);
    target.in(channel);

    sc_start(); //run simulation

    return 0;
}
