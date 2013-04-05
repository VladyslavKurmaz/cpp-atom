#pragma once

class cons_mpp : public boost::noncopyable {
public:
	///
	cons_mpp();
	///
	~cons_mpp();

protected:
private:
	///
#pragma warning ( push )
#pragma warning ( disable : 4200 )
	struct shared_block_t {
		unsigned int
			locale;
		UINT
			input_cp;
		UINT
			output_cp;
		CHAR_INFO
			csb[0];
	} *shared_block;
#pragma warning ( pop )
	boost::shared_array< unsigned char >
		shared_block_arr;
};