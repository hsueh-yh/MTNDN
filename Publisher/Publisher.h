#include <cstring>

#include <ndn-cpp/face.hpp>
#include <ndn-cpp/security/key-chain.hpp>


#define WIDTH 640
#define HEIGHT 480


using namespace ndn;
using namespace std;


class Publisher {
public:
	Publisher ( KeyChain &keyChain, const Name& certificateName ) :
		keyChain_ ( keyChain ),
		certificateName_ ( certificateName ),
		responseCount_ ( 0 ),
		count ( 0 )
	{
		ifp = fopen ( "out.264", "rb" );
		if ( ifp == NULL )
			cout << "Open file error!" << endl;
	}


	~Publisher ()
	{
		fclose ( ifp );
	}

	// onInterest.
	void operator()
					(const ptr_lib::shared_ptr<const Name>& prefix,
					const ptr_lib::shared_ptr<const Interest>& interest, Face& face,
					uint64_t interestFilterId,
					const ptr_lib::shared_ptr<const InterestFilter>& filter);

	// onRegisterFailed.
	void operator()(const ptr_lib::shared_ptr<const Name>& prefix);


	int start();
	int stop();

	KeyChain keyChain_;
	//Name myPrefix;
	Name certificateName_;
	int responseCount_;

	int stat;

	FILE* ifp;
	int count;


public:
	static int FindStartCode ( unsigned char *Buf, int zeros_in_startcode )
	{
		int info;
		int i;

		info = 1;
		for ( i = 0; i < zeros_in_startcode; i++ )
		{
			if ( Buf[i] != 0 )
				info = 0;
		}

		if ( Buf[i] != 1 )
			info = 0;
		return info;
	}

	static bool Check_StartCode ( unsigned char *Buf, int pos )
	{
		int info3 = 0;

		info3 = FindStartCode ( &Buf[pos - 4], 3 );
		return info3 == 1;

	}

	static int getNextNal ( FILE* inpf, unsigned char* Buf )
	{
		int pos = 0;
		int StartCodeFound = 0;
		int info2 = 0;
		int info3 = 0;

		int nCount = 0;
		while ( !feof ( inpf ) && ++nCount <= 4 )
		{
			Buf[pos++] = fgetc ( inpf );
		}

		if ( !Check_StartCode ( Buf, pos ) )
		{
			return 0;
		}


		while ( !feof ( inpf ) && ( Buf[pos++] = fgetc ( inpf ) ) == 0 );

		//find the next start code
		while ( !StartCodeFound )
		{
			//end of file
			if ( feof ( inpf ) )
			{
				//			return -1;
				return pos - 1;
			}
			Buf[pos++] = fgetc ( inpf );

			StartCodeFound = Check_StartCode ( Buf, pos );
		}

		fseek ( inpf, -4, SEEK_CUR );

		// return the end(length) of this NALU
		return pos - 4;
	}


};
