#ifndef ARCFACEDEPLOY_H
#define ARCFACEDEPLOY_H

//#include <qt_windows.h>
//#include <wingdi.h>

//#ifdef _UNICODE
//#if defined _M_IX86
//#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
//#elif defined _M_X64
//#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
//#else
//#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
//#endif
//#endif

#ifdef _M_IX86
#define APPID "Dd3XB3xdnMAE3PrivAwuqaSR8ZE8uPkHtxUvbCW7kYvB"
#define FT_SDKKEY		"3XQ8jJ2oGye4Zov9VCpqCvwK1zMRxCV4s1w1VNtpZ3bv"
#define FD_SDKKEY		"3XQ8jJ2oGye4Zov9VCpqCvwSBPcZbeMw2N1AHZxm2kgF"
#define FR_SDKKEY		"3XQ8jJ2oGye4Zov9VCpqCvwvpzfHtD4oT8hKdU4JYaP5"
#define AGE_SDKKEY		"3XQ8jJ2oGye4Zov9VCpqCvxB9oBeuBhnuLC1yTSkGk1a"
#define GENDER_SDKKEY	"3XQ8jJ2oGye4Zov9VCpqCvxJKCSkTGqH9nz9H5pVb3fD"
#elif defined _M_X64
#define APPID "Dd3XB3xdnMAE3PrivAwuqaSR8ZE8uPkHtxUvbCW7kYvB"
#define FT_SDKKEY		"3XQ8jJ2oGye4Zov9VCpqCvwK1zMRxCV4s1w1VNtpZ3bv"
#define FD_SDKKEY		"3XQ8jJ2oGye4Zov9VCpqCvwSBPcZbeMw2N1AHZxm2kgF"
#define FR_SDKKEY		"3XQ8jJ2oGye4Zov9VCpqCvwvpzfHtD4oT8hKdU4JYaP5"
#define AGE_SDKKEY		"3XQ8jJ2oGye4Zov9VCpqCvxB9oBeuBhnuLC1yTSkGk1a"
#define GENDER_SDKKEY	"3XQ8jJ2oGye4Zov9VCpqCvxJKCSkTGqH9nz9H5pVb3fD"
#endif

#endif // ARCFACEDEPLOY_H
