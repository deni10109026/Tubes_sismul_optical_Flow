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
