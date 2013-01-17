#include <stdio.h>
#include <cv.h>
#include <highgui.h>
#include <math.h>
static const double pi = 3.14159265358979323846;

inline static double square(int a)
{
return a * a;
}

inline static void allocateOnDemand( IplImage **img, CvSize size, int depth, int channels
)
{
if ( *img != NULL ) return;
*img = cvCreateImage( size, depth, channels );
if ( *img == NULL )
{
fprintf(stderr, "Error: Tidak dapat mengalokasikan gambar\n");
exit(-1);
}
}
int main(void)
{
    /* Input video dari kamera */
    CvCapture* input_video;
    input_video = cvCaptureFromCAM(CV_CAP_ANY);
    /* cek keserdiaan kamera */
    if (input_video == NULL)
        {
            fprintf(stderr, "Error: Kamera tidak terdeteksi.\n");
    return -1;
}
    /* mengambil frame dari video */
    cvQueryFrame( input_video );
    /* mengambil properti dari video */
    CvSize frame_size;
    frame_size.height = (int) cvGetCaptureProperty( input_video, CV_CAP_PROP_FRAME_HEIGHT );
    frame_size.width = (int) cvGetCaptureProperty( input_video, CV_CAP_PROP_FRAME_WIDTH );
    /* membuat window baru bernama optical flow */
        cvNamedWindow("Optical Flow", CV_WINDOW_AUTOSIZE);
        long current_frame = 0;
    while(1)
{
        static IplImage *frame = NULL, *frame1 = NULL, *frame1_1C = NULL, *frame2_1C = NULL, *eig_image = NULL, *temp_image = NULL, *pyramid1 = NULL, *pyramid2 = NULL;
        /* mendapatkan frame selanjutnya */
    frame = cvQueryFrame( input_video );
/* mengalokasikan gambar */
    	allocateOnDemand( &frame1_1C, frame_size, IPL_DEPTH_8U, 1 );
		cvConvertImage(frame, frame1_1C, 0);
		allocateOnDemand( &frame1, frame_size, IPL_DEPTH_8U, 3 );
		cvConvertImage(frame, frame1, 0);

		/* mendapatkan frame ke dua */
		frame = cvQueryFrame( input_video );
		allocateOnDemand( &frame2_1C, frame_size, IPL_DEPTH_8U, 1 );
		cvConvertImage(frame, frame2_1C, 0);

		/* Shi and Tomasi Feature Tracking! */
		/* mengalokasikan gambar */
		allocateOnDemand( &eig_image, frame_size, IPL_DEPTH_32F, 1 );
		allocateOnDemand( &temp_image, frame_size, IPL_DEPTH_32F, 1 );

		/* Preparation: This array will contain the features found in frame 1. */
		CvPoint2D32f frame1_features[400];

		/* menginisialisasi jumlah feature / garis panah */
		int number_of_features;
		number_of_features = 400;

		/* menjalankan algoritma Shi dan Tomasi */
		cvGoodFeaturesToTrack(frame1_1C, eig_image, temp_image, frame1_features, & number_of_features, .01, .01, NULL);

		/* Pyramidal Lucas Kanade Optical Flow! */
		/* menyimpan lokasi poin dari frame 1 di frame 2 dalam array */
		CvPoint2D32f frame2_features[400];
		char optical_flow_found_feature[400];
		float optical_flow_feature_error[400];

		CvSize optical_flow_window = cvSize(3,3);

		CvTermCriteria optical_flow_termination_criteria = cvTermCriteria( CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 20, .3 );

		allocateOnDemand( &pyramid1, frame_size, IPL_DEPTH_8U, 1 );
		allocateOnDemand( &pyramid2, frame_size, IPL_DEPTH_8U, 1 );

		 /* menjalakan Pyramidal Lucas Kanade Optical Flow */
		cvCalcOpticalFlowPyrLK(frame1_1C, frame2_1C, pyramid1, pyramid2, frame1_features, frame2_features, number_of_features, optical_flow_window, 5, optical_flow_found_feature, optical_flow_feature_error, optical_flow_termination_criteria, 0 );
