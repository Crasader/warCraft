//
//  gmfActions.h
//
//  Created by GMobi on 14-3-11.
//
//

#import <Foundation/Foundation.h>


#define  GMF_SSO_CONNECT   @"sso/connect"
#define  GMF_SSO_REGISTER  @"sso/register"
#define  GMF_SSO_LOGIN     @"sso/login"
#define  GMF_SSO_CHECK     @"sso/check"
#define  GMF_SSO_LOGOUT     @"sso/logout"

#define  GMF_PARAM_USERNAME   @"username"
#define  GMF_PARAM_PASSWORD   @"password"
#define  GMF_PARAM_TRACK_ID   @"track_id"
#define  GMF_PARAM_MCC        @"gmobi_mcc"
#define  GMF_PARAM_PROVIDER   @"gmobi_provider"
#define  GMF_PARAM_CPUUID     @"cp_uuid"


#define  GMF_RESULT_USER_ID        @"user_id"
#define  GMF_RESULT_USER_NAME      @"user_name"
#define  GMF_RESULT_USER_REGISTER  @"user_register"
#define  GMF_RESULT_USER_DETAILS   @"user_details"
#define  GMF_RESULT_EXISTS         @"exists"
#define  GMF_RESULT_TRACK          GMF_PARAM_TRACK_ID
#define  GMF_RESULT_ERROR_CODE     @"error_code"
#define  GMF_RESULT_EXTRA            @"result_extra"
#define  GMF_RESULT_PROVIDER       @"user_provider"

typedef void (^GMFAppCallHandler)(BOOL success, NSDictionary* resultData);

