#ifndef W_cvThreshold_H
#define W_cvThreshold_H

#include <cvVideoOp.h>
#include <wEnum.h>

wEnum *cvThresholdType = new wEnum("threshold_type",
		"binary", CV_THRESH_BINARY,
		"binary_inv", CV_THRESH_BINARY_INV,
		"truncate", CV_THRESH_TRUNC,
		"tozero", CV_THRESH_TOZERO,
		"tozero_inv", CV_THRESH_TOZERO_INV,
		NULL
	);


class wcvThreshold : public cvVideoOp {
	protected:
		wFloatProperty threshold, maximum;
		wEnumProperty type;

	public:
		wcvThreshold()
				: cvVideoOp()
				, threshold( "threshold","threshold level", 0x7f, 0, 0xff )
				, maximum( "max","maximum level", 0xff, 0, 0xff )
				, type( "type","threshold type", cvThresholdType->Type(), CV_THRESH_BINARY )
				{
				AddProperty( threshold );
				AddProperty( maximum );
				AddProperty( type );
			}

		virtual bool Process( IplImage *in, IplImage *out ) {
				cvThreshold( in, out, threshold.Get(), maximum.Get(), type.Get() );
				return TRUE;
			}
};

#endif
