#ifndef W_wBaseSrc_H
#define W_wBaseSrc_H

#include <gst/base/gstbasesrc.h>
#include <wElement.h>

class wBaseSrc : public wElement<GstBaseSrc,GstBaseSrcClass> {
	public:
		wBaseSrc() {
			
			}

	protected:
	
	public:
		static GType get_parent_type() {
				return GST_TYPE_BASE_SRC;
			}
};

#endif
