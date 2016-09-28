#include <iostream>

#include "capturer.hpp"

using namespace std;

int main()
{
    Capturer cap;

    FILE *fp_yuv, *fp_264;

    int flg = 1;

    cout << "********************************************" << endl;
    fp_yuv=fopen("output.yuv","wb+");
    if( !fp_yuv )
    {
        cout << "open file fp_yuv error\n" << endl;
    }
    else
        cout << "open file fp_yuv success\n" << endl;

    
    cap.init();
    cap.start();
    
    unsigned char *out_YUV_buffer = NULL;

    int width, height;

    for( int i = 0; i < 200; i++ )
    {
        cap.getAVFrame( width, height );
        out_YUV_buffer = out_buffer;

        cout << endl << width << " " << height << endl << endl;
        if( out_YUV_buffer != NULL )
        {
            cout << "Got YUV"<<endl;
            int y_size = width * height*3/2;
            fwrite(out_YUV_buffer,1,y_size,fp_yuv);
        }
        else
        {
            cout << "No YUV" << endl;
            return -1;
        }

    }

    cout << "********************************************" << endl;
    fclose(fp_yuv);

    cap.stop();

    return 0;
}



