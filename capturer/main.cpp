#include <iostream>

#include "capturer.hpp"
#include "encoder.hpp"

using namespace std;

int main()
{
    Capturer cap;
    Encoder encoder;
    int width = 640, height = 480;

    int isencode = 1;

    FILE *fp_yuv, *fp_264;

    unsigned char* outbufYUV = NULL;
    int outlenYUV = 0;
    unsigned char* outbuf264 = NULL;
    int outlen264 = 0;

    int flg = 1;

    cout << endl;

    fp_yuv=fopen("output.yuv","wb+");
    if( !fp_yuv )
    {
        cout << "open file fp_yuv error" << endl;
    }
    else
        cout << "open file fp_yuv success" << endl;

    if( isencode )
    {
        fp_264=fopen("output.264","wb+");
        if( !fp_264 )
        {
            cout << "open file output.264 error" << endl;
        }
        else
            cout << "open file output.264 success" << endl;
    }

    cap.init();
    cap.start();

    if ( isencode )
    {
        encoder.init(AV_CODEC_ID_H264);
    }

    outbufYUV = (unsigned char*) malloc (width*height*3/2);

    if( isencode) outbuf264 = (unsigned char*) malloc (width*height*3/2);

    cout << "********************************************" << endl;

    for( int i = 0, j = 0; i < 100; i++ )
    {
        cap.getFrame(outbufYUV, outlenYUV );

        if( outlenYUV != 0 )
        {
            cout << "get YUV " << i <<endl;
            fwrite(outbufYUV,1,outlenYUV,fp_yuv);
        }
        else
        {
            cout << "error" << endl;
            return -1;
        }

        if( isencode && outlenYUV != 0 )
        {
            encoder.getFrame(i, outbufYUV, outlenYUV, outbuf264, outlen264);
            if( outlen264 != 0 )
            {
                cout << "get 264 " << j++ << " ( size = " << outlen264 << " )" <<endl;
                fwrite(outbuf264,1,outlen264,fp_264);
            }
            else
                cout << "not 264" << endl;
        }
        //cout << endl;
    }
    cout << "********************************************" << endl<<endl;

    fclose(fp_yuv);
    if ( isencode )
    {
        fclose(fp_264);
        encoder.stop();
    }

    cap.stop();


    return 0;
}


